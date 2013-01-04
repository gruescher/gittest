
/** Klasse zu Anfordern und Freigeben von Shared Memory und
 *  von Mutex-Objekten die �ber CPU-Grenzen hinweg nutzbar sind.
 *
 *  <B>Shared Memory</B><BR>
 *  Diese Klasse kann verschiedene Typen von Shared Memory anlegen und verwalten:
 *
 *  - "Lokales" Shared Memory auf welches Prozessen/Threads auf einer CPU zugreifen k�nnen
 *  - Node-�bergreifendes Shared Memory auf welches von mehreren, getrennten CPUs zugegriffen werden kann
 *  - Shared Memory im dSPACE "Global Mem" auf welches HostPC und Targetsystem zugreifen k�nnen
 *
 *  Das Node/CPU-�bergreifende Shared Memory ist derzeit f�r den Einsatz zwischen CN und HCN
 *  des Echtzeitsystems konzipiert.  Ein solches Cross-Node Shared Memory kann hardwarem��ig
 *  auf verschiedene Arten implementiert sein (im FPGA, per PCI-Express, per Serial Rapid-IO).
 *  Bei Implementierung per PCIe oder SRIO liegt ein Shared Memory Bereich typischerweise im RAM
 *  von CN oder HCN und kann dann von der Gegenstelle seriell gelesen und beschrieben werden.
 *  In diesem Fall sind die Zugriffe der lokalen CPU so schnell wie RAM, w�hrend die remote-Zugriffe
 *  eher langsam sind (typischer zwischen 50 und 250 MB/sec bei Latenzen von 2-4 us pro Zugriff)
 *
 *  Aus diesem Grund muss bei der Allokation von Cross-Node Shared Memory mit angegeben werden,
 *  wo der allokierte Spicher physikalisch liegen soll (im RAM des CN oder im RAM des HCN)
 *
 *
 *  <B>Cross-CPU Mutexe</B><BR>
 *  Diese Klasse bietet ein Mutex Objekt an, mit dem kritische Abschnitte CPU-�bergreifend
 *  verriegelt werden k�nnen.  Ein solcher Mutex bietet die typischen Funktionen wie WaitLock()
 *  TryLock() und Release() - mit und ohne Timeout.
 *
 */
#ifndef __DS_SHAREDMEMORY_H__
#define __DS_SHAREDMEMORY_H__

/**
 * @ingroup SharedMemoryAttributes
 * @name Shared Memory Attributes
 * @{
 */

/** Speicher wird gecached eingemappt
 *  Gecachtes Shared Memory wird nicht automatisch in den Zielspeicher geschrieben
 *  und ist generell nur lokal innerhalb einer CPU verwendbar.
 */
#define   SHM_ATTR_CACHED                   (1<<0)

/** Speicher wird read-only eingemappt.
 *  Schreibversuche auf diesen Speicher f�hren zu einer QNX Exception
 */
#define   SHM_ATTR_RDONLY                   (1<<1)

/** Speicher wird als "Applikationsspezfisch" markiert.
 *  Dieses Flag kann von h�heren Schichte verwendet werden, um
 *  Speicher zu erkennen der nur g�ltig ist, solange eine Appliation l�uft.
 */
#define   SHM_ATTR_APPLICATION              (1<<2)

/**
 * @}
 */


/**
 * @ingroup SharedMemoryLocations
 * @name Speicherorte eines Shared Memory Abschnitts
 * @{
 */

/** "Lokales" Shared Memory
 *  Shared Memory dieses Typs ist nur innerhalb einer Node (CPU) verf�gbar
 *  und daher nur f�r lokale Interprozesskommunikatin geeignet.
 */
#define   SHM_LOCATION_LOCAL                   0

/** Shared Memory Abschnitt auf dem CN
 *  Shared Memory dieses Typs wird auf dem CN gespeichert und kann dort performant gelesen und beschrieben werden.
 *  Zugriffe von anderen Nodes (HCN) aus sind hinsichtlich Durchsatz und Latenz weniger effizient.
 */
#define   SHM_LOCATION_ON_CN                   1

/** Shared Memory Abschnitt auf dem HCN
 *  Shared Memory dieses Typs wird auf dem HCN gespeichert und kann dort performant gelesen und beschrieben werden.
 *  Zugriffe von anderen Nodes (HCN) aus sind hinsichtlich Durchsatz und Latenz weniger effizient.
 */
#define   SHM_LOCATION_ON_HCN                  2

/** "Globales" Shared Memory
 *  Shared Memory dieses Typs liegt im klassischen dSPACE "Global Memory" und kann
 *  sowohl vom Echtzeitsystem als auch vom Host PC gelesen und geschrieben werden.
 */
#define   SHM_LOCATION_GLOBAL                  3

/**
 * @}
 */


/**
 * @ingroup SharedMemoryOpenMode
 * @name Modi f�r "open" Funktion des DsSharedMemory Moduls.
 * @{
 */

/** Versuche, einen Shared Memory zu �ffnen.
 *  Falls der Bereich nicht existiert, wird er neu angelegt.
 */
#define   SHM_OPEN_CREATE_IF_MISSING          1

/** Versuche, einen Shared Memory zu �ffnen.
 *  Falls der Bereich nicht existiert, wird ein Fehler gemeldet.
 */
#define   SHM_OPEN_FAIL_IF_MISSING            2

/** Versuche, einen Shared Memory neu anzulegen.
 *  Falls der Bereich bereits existiert, wird ein Fehler gemeldet.
 */
#define   SHM_CREATE_FAIL_IF_EXISTING         3

/**
 * @}
 */



/**
 * @ingroup SharedMemoryErrorCodes
 * @name Fehlercodes des DsSharedMemory Moduls.
 * @{
 */

/// Error Code: Shared Memory oder Mutex konnte nicht allokiert werden
#define   SHM_ERR_OUT_OF_MEMORY                -1
/// Error Code: Shared Memory Bereich eines Namens konnte nicht gefunden werden
#define   SHM_ERR_MEM_NOT_FOUND                -2
/// Shared Memory Bereich konnte nicht angelegt werden da er schon existiert
#define   SHM_ERR_MEM_ALREADY_EXISTS           -3
/// Error Code: Unzul�ssiger Paramter: Speicheort des Shared Memory Bereichs
#define   SHM_ERR_INVALID_LOCATION             -4
/// Error Code: Unzul�ssiger Paramter: Creation-Flags des Shared Memory Bereichs
#define   SHM_ERR_INVALID_CREATEFLAGS          -5
/// Error Code: Mutex konnte nicht angefordert werden.
#define   SHM_ERR_LOCK_FAILED                  -6
/// Error Code: Timeout beim Anfordern eine Mutex
#define   SHM_ERR_LOCK_TIMEOUT                 -7
/// Error Code: Mutex konnte nicht zur�ck gegeben werden.
#define   SHM_ERR_UNLOCK_FAILED                -8
/// Error Code: Mutex-Speicher konnte nicht frei gegeben werden.
#define   SHM_ERR_FREE_FAILED                  -9

/**
 * @}
 */



/**
 * @ingroup SharedMemoryApi
 * @name Shared Memory API
 * @{
 */

/**
 *  Shared Memory Descriptor.
 *  Beschreibt einen Shared Memory Bereich vollst�ndig.
 */
typedef struct _T_SHM_DESCRIPTOR
{
    const char      *Name;              /// Name des Shared Memory Bereiches
    UInt32           ID;                /// Frei w�hlbare ID des Shared Memory Bereich es
    UInt32           Size;              /// Gr��e in Bytes
    void            *MemPtr;            /// Virtuelle Startadresse
    ULong64          PhysAddress;       /// Physikalische Startadresse
    int              Location;          /// Speicherort (siehe SHM_LOCATION_xxx)
    int              Attributes;        /// Attributsbits (siehe SHM_ATTR_xxx)
} T_SHM_DESCRIPTOR;


/**  Initialize everything
 */
void initSharedMemoryHeaps
    (
        bool    forceReset
    );




/** �ffnet und/oder erzeugt einen Shared Memory Bereich.
 *
 *  Mit dieser Funktion kann ein Shared Memory Bereich neu angelegt oder
 *  ein existierende Bereich ge�ffnet werden.  Das genaue Verhalten,
 *  d.h. ob ge�ffnet und/oder erzeugt wird, wird durch den Parameter
 *  CreateFlags festgelegt (s.d.).
 *
 *  Aufrufparameter diese Funktion:
 *
 *  @param ppSharedMemory       R�ckgabewert: Enth�lt im Erfolgsfall Zeiger auf den allokierten/ge�ffneten Shared Memory Bereich
 *  @param pSize                R�ckgabewert: Enth�lt im Erfolgsfall die Gr��e des allokierten/ge�ffneten Shared Memory Bereiches
 *
 *  @param StrName              Name des Bereichs als C-String (z.B. "DsAppMgrShm")
 *  @param Id                   Frei w�hlbare, 32-bittige ID (z.B. eine Application ID).
 *  @param Size                 Anzahl zu allokierender Bytes
 *  @param Location             Speicherort.  Folgende Speicherort werden unterst�tzt:
 *                                  - SHM_LOCATION_LOCAL    Lokaler Speicher, der nur innerhalb einer Node(CPU) geshared werden
 *                                  - SHM_LOCATION_ON_CN    Speicher, auf den der CN performant zugreifen kann.  Zugriffe vom HCN sind potenziell langsamer.
 *                                  - SHM_LOCATION_ON_HCN   Speicher, auf den der HCN performant zugreifen kann.  Zugriffe vom CN sind potenziell langsamer.
 *                                  - SHM_LOCATION_GLOBAL   dSPACE "GlobalMem" zwischen HostPC und Bestandssystemen
 *  @param AttributeFlags       Optionale, frei kombinierbare Flags f�r diesen Bereich:
 *                                  - SHM_ATTR_CACHED       Dieser Bereich soll gecached eingemappt werden
 *                                  - SHM_ATTR_RDONLY       Dieser Bereich soll read-only eingemappt werden
 *                                  - SHM_ATTR_APPLICATION  Dieser Bereich soll als "Application Shared Mem" markiert werden.
 *                                                          Dieses Flag wird von der SharedMem Speicherverwaltung nicht ausgewertet
 *                                                          sondern steht nur den aufrufenden Schichten zur Verf�gung.
 * @param CreateFlags           Legt fest, ob und der Bereich ge�ffnet und/oder erzeugt werden soll:
 *                                  - SHM_OPEN_CREATE_IF_MISSING    Versucht, den Bereich zu �ffnen.  Legt ihn an falls er nicht existiert.
 *                                  - SHM_OPEN_FAIL_IF_MISSING      Versucht, den Bereich zu �ffnen.  Gibt einen Fehler zur�ck, falls er nicht existiert.
 *                                  - SHM_CREATE_FAIL_IF_EXISTING   Versucht, den Bereich zu erzeugen.  Gibt einen Fehler zur�ck, falls er bereits existiert.
 *
 * Grunds�tzlich gilt: Bei der �berpr�fung, ob ein Shared Memory Bereich bereits existiert,
 * werden die Parameter Name, ID und Location hinzugezogen.  Attribute und Gr��e werden nicht
 * gepr�ft.  Auf diese Weise kann man z.B. f�r einen bereits existierenden Bereich die tats�chliche Gr��e abfragen.
 *
 * @return Fehlercode.  M�glich sind:
 *            - 0                             Kein Fehler
 *            - SHM_ERR_OUT_OF_MEMORY         Nicht gen�gend freier Speicherplatz
 *            - SHM_ERR_MEM_NOT_FOUND         Shared Memory nicht gefunden (relevant falls CreateFlags==SHM_OPEN_FAIL_IF_MISSING)
 *            - SHM_ERR_MEM_ALREADY_EXISTS    Shared Memory existiert bereit (relevant falls CreateFlags==SHM_CREATE_FAIL_IF_EXISTING)
 *            - SHM_ERR_INVALID_LOCATION      Parameter "Location" falsch
 *            - SHM_ERR_INVALID_CREATEFLAGS   Parameter "CreateFlags" falsch
 */
int openSharedMemory
    (   void           **ppSharedMemory,
        UInt32          *pSize,

        const char      *StrName,
        UInt32           Id,
        UInt32           Size,
        int              Location,
        int              AttributeFlags,
        int              CreateFlags
    );



/** Pr�ft, ob ein Shared Memory Bereich existiert.
 *
 *  Mit dieser Funktion kann gepr�ft werden, ob ein Shared Memory Bereich mit gegebenem
 *  Namen, ID und Speicherort bereits existiert.
 *
 *  Aufrufparameter diese Funktion:
 *
 *  @param ppShmDescriptor      Zeiger auf Descriptor in dem im Erfolgsfall die Attributen des Shared Memory Bereichs abgelegt werden.
 *                              Im Fehlerfall wird der Descriptor nicht ver�ndert.  Wird als Zeiger NULL �bergeben, so werden die
 *                              Attribute nicht zur�ckgegeben.
 *
 *  @param StrName          Name des zu suchenden Bereichs als C-String (z.B. "DsAppMgrShm")
 *  @param Id               Id des zu suchenden Bereiches
 *  @param Location         Speicherort des zu suchenden Bereiches.  Folgende Speicherorte werden unterst�tzt:
 *                              - SHM_LOCATION_LOCAL    Lokaler Speicher
 *                              - SHM_LOCATION_ON_CN    Speicher auf dem CN
 *                              - SHM_LOCATION_ON_HCN   Speicher auf dem HCN
 *                              - SHM_LOCATION_GLOBAL   dSPACE "GlobalMem"
 *
 * @return Fehlercode.  M�glich sind:
 *            - 0                             Shared Memory Bereich existiert
 *            - SHM_ERR_MEM_NOT_FOUND         Shared Memory Bereich existiert nicht
 *            - SHM_ERR_INVALID_LOCATION      Parameter "Location" falsch
 *
 */
int checkSharedMemory
    (   T_SHM_DESCRIPTOR    *ppShmDescriptor,
        const char          *StrName,
        UInt32              Id,
        int                 Location
    );


/** Gibt einen Shared Memory Bereich frei.
 *
 *  Mit dieser Funktion wird ein Shared Memory Bereich freigegeben.
 *  Der zu schlie�ende Bereich wird durch seine Startadresse identifiziert.
 *
 *  Beim Schlie�en eines Shared Memory Bereiches wird dieser aus dem
 *  aufrufenden Prozessraum ausgemappt.  Sobald der Bereich von allen
 *  Teilnehmern ausgemappt wurde, wird er freigegeben.
 *
 *  Aufrufparameter diese Funktion:
 *
 *  @param pSharedMemory      Startadresse des zu schlie�enden Shared Memory Bereiches.
 *  @param Location           Speicherort des freizugebenden Bereiches.  Folgende Speicherorte werden unterst�tzt:
 *                              - SHM_LOCATION_LOCAL    Lokaler Speicher
 *                              - SHM_LOCATION_ON_CN    Speicher auf dem CN
 *                              - SHM_LOCATION_ON_HCN   Speicher auf dem HCN
 *                              - SHM_LOCATION_GLOBAL   dSPACE "GlobalMem"
 *  @param ForceRemove        1, falls eine Freigabe des Bereichs erzwungen werden soll
 *
 * @return Fehlercode.  M�glich sind:
 *            - 0                             Shared Memory Bereich erfolgreich geschlossen
 *            - SHM_ERR_MEM_NOT_FOUND         Shared Memory Bereich existiert nicht
 */
int closeSharedMemory
    (   void           *pSharedMemory,
        int             Location,
        int             ForceRemove
     );


/** Gibt einen Shared Memory Bereich frei.
 *
 *  Mit dieser Funktion wird ein Shared Memory Bereich freigegeben.
 *  Der zu schlie�ende Bereich wird durch seine Namen, seine Id und
 *  seinen Speicherort identifiziert.
 *
 *  Beim Schlie�en eines Shared Memory Bereiches wird dieser aus dem
 *  aufrufenden Prozessraum ausgemappt.  Sobald der Bereich von allen
 *  Teilnehmern ausgemappt wurde, wird er freigegeben.
 *
 *  Aufrufparameter diese Funktion:
 *
 *  @param StrName          Name des freizugebenden Bereichs als C-String (z.B. "DsAppMgrShm")
 *  @param Id               Id des freizugebenden Bereiches
 *  @param Location         Speicherort des freizugebenden Bereiches.  Folgende Speicherorte werden unterst�tzt:
 *                              - SHM_LOCATION_LOCAL    Lokaler Speicher
 *                              - SHM_LOCATION_ON_CN    Speicher auf dem CN
 *                              - SHM_LOCATION_ON_HCN   Speicher auf dem HCN
 *                              - SHM_LOCATION_GLOBAL   dSPACE "GlobalMem"
 *  @param ForceRemove      1, falls eine Freigabe des Bereichs erzwungen werden soll
 *
 * @return Fehlercode.  M�glich sind:
 *            - 0                             Shared Memory Bereich erfolgreich geschlossen
 *            - SHM_ERR_MEM_NOT_FOUND         Shared Memory Bereich existiert nicht
 */
int closeSharedMemory
    (   const char     *StrName,
        UInt32          Id,
        int             Location,
        int             ForceRemove
     );

/** Gibt einer Liste aller an einem bestimmten Speicherort allokieren Shared Memory Bereiche zur�ck.
 *
 *  Aufrufparameter diese Funktion:
 *
 *  @param ppShmList        R�ckgabewert: Erh�lt im Erfolgsfall Zeiger auf Array mit Shared Memory Descriptoren
 *  @param pNumEntries      R�ckgabewert: Erh�lt im Erfolgsfall Anzahl der ELemente im Array *ppShmList
 *  @param Location         Gibt an, welche Speicherort durchsucht werden soll.  Folgende Speicherorte werden unterst�tzt:
 *                              - SHM_LOCATION_LOCAL    Lokaler Speicher, der nur innerhalb einer Node(CPU) geshared werden
 *                              - SHM_LOCATION_ON_CN    Speicher, auf den der CN performant zugreifen kann.  Zugriffe vom HCN sind potenziell langsamer.
 *                              - SHM_LOCATION_ON_HCN   Speicher, auf den der HCN performant zugreifen kann.  Zugriffe vom CN sind potenziell langsamer.
 *                              - SHM_LOCATION_GLOBAL   dSPACE "GlobalMem" zwischen HostPC und Bestandssystemen
 *
 *  Nach dem Auswerten der in *ppShmList zur�ckgegebenen Liste sollte der Aufrufer den f�r die
 *  Liste allokierten Speicher mit free(*ppShmList) wieder freigeben.
 *
 *  @return Fehlercode.  M�gliche Werte sind:
 *              - 0                          Liste wurde erfolgreich erzeugt
 *              - SHM_ERR_INVALID_LOCATION   Parameter "Location" falsch
 */
int listSharedMemory
    (   T_SHM_DESCRIPTOR   **ppShmList,
        UInt32              *pNumEntries,
        int                  Location
    );



/**  Debug only
 */
void dumpSharedMemoryHeap
    (
        int                  Location
    );


/**  Handle auf Cross-Node-Mutex
 */
typedef void *T_DsCrossNodeMutex;


/*
 *    Mutex Funktions-API
 */

/** Allokiert einen Mutext der f�r Cross-Node(CPU) Synchronisation geeignet ist.
 *  Ein Cross-Node-Mutex ist ein Prozesssynchronisationsobjekt mit dem zeitliche
 *  Abl�ufe �ber CPU-Grenzen hinweg oder lokal auf einer CPU synchronisiert werden kann.
 *
 *  Diese Funktion allokiert einen Mutex falls er noch nicht existiert bzw. gibt den
 *  Zeiger auf den Mutex zur�ck falls dieser zuvor schon angelegt wurde.
 *
 *  Aufrufparameter diese Funktion:
 *
 *  @param ppMutex      R�ckgabewert: Erh�lt im Erfolgsfall Zeiger auf den neu allokierten Mutex.
 *  @param StrName      Name des zu allokierenden Mutex
 *
 *  @return Fehlercode.  M�gliche Werte sind:
 *              - 0                       Mutex wurde erfolgreich geholt/erzeugt
 *              - SHM_ERR_OUT_OF_MEMORY   Mutex existiert nicht und nicht gen�gend Speicher um ihn zu erzeugen.
 */
int allocateCrossNodeMutex
    (   T_DsCrossNodeMutex     **ppMutex,
        const char              *StrName
    );

/** Fordert einen Mutex an ("lockt den Mutex")
 *
 *  Diese Funktion fordert einen Mutex an und wartet so lange, bis
 *  der Mutex frei ist oder bis ein gegebener Timeout-Wert �berschritten ist.
 *  Der Aufrufer kann zudem w�hlen, ob die Funktion blockieren darf oder
 *  per busy-Waiting warten soll (was z.B. in IRQ-Handler notwendig sein k�nnte)
 *
 *  Aufrufparameter diese Funktion:
 *
 *  @param pMutex                   Zeiger auf den anzufordernden Mutex.
 *  @param TimeoutMicroseconds      Maximale Wartezeit in ms.  0 hei�t endloses Warten bis der Mutex frei wird.
 *  @param MayBlock                 1 falls die Funktion blockieren darf, 0 falls sie durch BusyWaiting/Spinning warten soll.
 *
 *  @return Fehlercode.  M�gliche Werte sind:
 *              - 0                      Mutex wurde erfolgreich angefordert.
 *              - SHM_ERR_LOCK_TIMEOUT  Timeout w�hrend auf Freigabe des Mutex gewartet wurde.
 */
int lockCrossNodeMutex
    (   T_DsCrossNodeMutex     *pMutex,
        UInt32                  TimeoutMicroseconds,
        int                     MayBlock
    );


/** Versucht, einen Mutex anzufordern ("lockt den Mutex")
 *
 *  Diese Funktion versucht, einen Mutex sofort anzufordern und zu locken.
 *  Ist der Mutex nichr frei, kehrt die Funktion sofort zur�ck
 *
 *  Aufrufparameter diese Funktion:
 *
 *  @param pMutex       Zeiger auf den anzufordernden Mutex
 *
 *  @return Fehlercode.  M�gliche Werte sind:
 *              - 0                      Mutex wurde erfolgreich angefordert.
 *              - SHM_ERR_LOCK_FAILED    Mutext ist nicht frei und konnte nicht angefordert werden.
 */
int tryLockCrossNodeMutex
    (   T_DsCrossNodeMutex     *pMutex
    );


/** Gibt einen zuvor angeforderten Mutex wieder zur�ck ("unlockt den Mutex")
 *
 *  Diese Funktion Gibt einen zuvor angeforderten Mutex wieder zur�ck.
 *
 *  Aufrufparameter diese Funktion:
 *
 *  @param pMutex       Zeiger auf den zur�ckzugebenden Mutex
 *
 *  @return Fehlercode.  M�gliche Werte sind:
 *              - 0                      Mutex wurde erfolgreich angefordert.
 *              - SHM_ERR_UNLOCK_FAILED  Mutext konnte nicht zur�ck gegeben werden.
 */
int releaseCrossNodeMutex
    (   T_DsCrossNodeMutex     *pMutex
    );


/** Gibt den Speicher eines Mutex wieder frei
 *
 *  Diese Funktion gibt den Speicher eines mit AllocateCrossCpuMutex() geholten
 *  Mutex wieder frei.
 *
 *  @param pMutex       Zeiger auf den Mutex dessen Speicher freigegeben werden soll
 *
 *  @return Fehlercode.  M�gliche Werte sind:
 *              - 0                     Speicher des Mutex wurde erfolgreich freigegeben.
 *              - SHM_ERR_FREE_FAILED   Speicher des Mutex konnte nicht freigegeben werden.
 */
int freeCrossNodeMutex
    (   T_DsCrossNodeMutex     *pMutex
    );


/** Gibt den Speicher eines Mutex wieder frei
 *
 *  Diese Funktion gibt den Speicher eines mit AllocateCrossCpuMutex() geholten
 *  Mutex wieder frei.
 *
 *  @param StrName       Name des Mutex dessen Speicher freigegeben werden soll
 *
 *  @return Fehlercode.  M�gliche Werte sind:
 *              - 0                     Speicher des Mutex wurde erfolgreich freigegeben.
 *              - SHM_ERR_FREE_FAILED   Speicher des Mutex konnte nicht freigegeben werden.
 */
int freeCrossNodeMutexByName
    (   const char           *StrName
    );

/**
 * @}
 */

#endif // __DS_SHAREDMEMORY_H__

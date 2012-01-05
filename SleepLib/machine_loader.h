/*

SleepLib MachineLoader Base Class Header

Author: Mark Watkins <jedimark64@users.sourceforge.net>
License: GPL

*/

#ifndef MACHINE_LOADER_H
#define MACHINE_LOADER_H
#include "profiles.h"
#include "machine.h"
#include "zlib.h"

/*! \class MachineLoader
    \brief Base class to derive a new Machine importer from
    */
class MachineLoader
{
public:
    MachineLoader();
    virtual ~MachineLoader();


    //virtual Machine * CreateMachine() {};

    //! \brief Override this to scan path and detect new machine data
    virtual int Open(QString & path,Profile *)=0;   // Scans for new content

    //! \brief Override to returns the Version number of this MachineLoader
    virtual int Version()=0;

    //! \brief Override to returns the class name of this MachineLoader
    virtual const QString & ClassName()=0;

    bool compressFile(QString inpath, QString outpath="");


 /*
    MachineLoader(Profile *profile,QString & classname);
    virtual void LoadMachineList();
    virtual void SaveMachineList();
    virtual bool LoadSummaries();
    virtual bool LoadEvents();
    virtual bool LoadWaveforms();
    virtual bool Scan(QString &)=0;   // Scans for new content

    virtual bool LoadAll();
    virtual bool SaveAll();

    virtual bool LoadSummary(Machine * m, QString & filename);
    virtual bool LoadEvent(Machine * m, QString & filename);
    virtual bool LoadWaveform(Machine * m, QString & filename);

    virtual bool SaveSummary(Machine * m, QString & filename);
    virtual bool SaveEvent(Machine * m, QString & filename);
    virtual bool SaveWaveform(Machine * m, QString & filename);*/

protected:
    //! \brief Contains a list of Machine records known by this loader
    QList<Machine *> m_machlist;
    QString m_class;
    MachineType m_type;
    Profile * m_profile;
};

// Put in machine loader class as static??
void RegisterLoader(MachineLoader *loader);
void DestroyLoaders();
QList<MachineLoader *> GetLoaders();

#endif //MACHINE_LOADER_H

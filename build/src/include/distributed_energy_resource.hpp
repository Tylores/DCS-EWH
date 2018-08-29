#ifndef DISTRIBUTEDENERGYRESOURCE_H
#define DISTRIBUTEDENERGYRESOURCE_H

class DistributedEnergyResource {
    public:
        // constructor / destructor
        DistributedEnergyResource (std::map <std::string, std::string> init);
        virtual ~DistributedEnergyResource ();
        void Loop (float delta_time);


    public:
        // mutators       
        void SetExportWatts (unsigned int power);
        void SetRatedExportPower (unsigned int watts);
        void SetRatedExportEnergy (unsigned int watt_hours);
        void SetExportRamp (unsigned int watts_per_second);
        void SetImportWatts (unsigned int power);
        void SetRatedImportPower (unsigned int watts);
        void SetRatedImportEnergy (unsigned int watt_hours);
        void SetImportRamp (unsigned int watts_per_second);
        void SetIdleLosses (unsigned int energy_per_hour);
        
    public:
        // accessors
        unsigned int GetRatedExportPower ();
        unsigned int GetExportPower ();
        unsigned int GetExportEnergy ();
        unsigned int GetExportRamp ();
        unsigned int GetRatedImportPower ();
        unsigned int GetImportPower ();
        unsigned int GetImportEnergy ();
        unsigned int GetImportRamp ();s
        unsigned int GetIdleLosses ();

    private:
        // controls
        // (TS): by making Import/Export Power virtual, they can be redefined
        //       within the derived class when inhearited. 
        virtual void ImportPower ();
        virtual void ExportPower ();
        void IdleLoss ();

    private:
        // static properties
        unsigned int rated_export_power_;       // (W) to grid
        unsigned int rated_export_energy_;      // (Wh)
        unsigned int export_ramp_;              // (W s^-1)
        unsigned int rated_import_power_;       // (W) from grid
        unsigned int rated_import_energy_;      // (Wh)
        unsigned int import_ramp_;              // (W s^-1)
        unsigned int idle_losses_;              // (Wh h^-1)

    private:
        // dynamic properties
        float export_power_;
        float export_energy_;
        float import_power_;
        float import_energy_;     

    private:
        // control properties
        unsigned int export_watts_;
        unsigned int import_watts_;
        float delta_time_;  // milliseconds
};

#endif // DISTRIBUTEDENERGYRESOURCE_H

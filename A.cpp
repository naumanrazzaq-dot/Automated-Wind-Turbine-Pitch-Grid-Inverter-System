#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

class WindAsset {
protected:
    string turbineID;
    bool isOperational;

public:
    static int activeTurbines;

    WindAsset(string id) : turbineID(id), isOperational(true) {
        activeTurbines++;
    }

    virtual ~WindAsset() {
        activeTurbines--;
    }

    virtual void transmitTurbineTelemetry() = 0;
};

int WindAsset::activeTurbines = 0;

class BladePitchModule : virtual public WindAsset {
protected:
    double bladePitchAngleDeg;

public:
    BladePitchModule(string id, double angle) : WindAsset(id), bladePitchAngleDeg(angle) {}

    void checkBladePitch() {
        cout << "Rotor pitch [" << turbineID << "] angle adjusted to " << bladePitchAngleDeg << " degrees." << endl;
    }
};

class GridInverterModule : virtual public WindAsset {
protected:
    double outputFrequencyHz;

public:
    GridInverterModule(string id, double frequency) : WindAsset(id), outputFrequencyHz(frequency) {}

    void checkInverter() {
        cout << "Grid inverter [" << turbineID << "] phase locked at " << outputFrequencyHz << " Hz." << endl;
    }
};

class OffshoreWindTurbine : public BladePitchModule, public GridInverterModule {
private:
    double generatedPowerMW;

public:
    OffshoreWindTurbine(string id, double angle, double frequency, double power)
        : WindAsset(id),
          BladePitchModule(id, angle),
          GridInverterModule(id, frequency),
          generatedPowerMW(power) {}

    ~OffshoreWindTurbine() {
        cout << "Turbine " << turbineID << " feathered and disconnected from offshore grid." << endl;
    }

    void transmitTurbineTelemetry() override {
        cout << "\n--- Offshore Wind Turbine Report: " << turbineID << " ---" << endl;
        checkBladePitch();
        checkInverter();
        cout << "Active Power Output: " << fixed << setprecision(2) << generatedPowerMW << " MW" << endl;
        cout << "Grid Sync Status: Online & Balanced" << endl;
    }
};

int main() {
    WindAsset* turbine = new OffshoreWindTurbine("WTG-NORTH-14", 14.5, 50.0, 12.75);

    turbine->transmitTurbineTelemetry();

    cout << "\nActive Offshore Turbines: " << WindAsset::activeTurbines << endl;

    delete turbine;

    cout << "Active Turbines After Shutdown: " << WindAsset::activeTurbines << endl;

    return 0;
}

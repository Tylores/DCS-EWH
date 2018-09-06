#include <iostream>
#include "include/electric_water_heater.hpp"
#include "easylogging++.h"

ElectricWaterHeater::ElectricWaterHeater (
	std::map <std::string, std::string> &init) 
	: DistributedEnergyResource(init), sp_("/dev/ttyUSB0") {
	if (!sp_.open ()) {
		LOG(ERROR) << "failed to open serial port: " << strerror(errno);
		exit (1);
	} else {
		device_ptr_ = cea2045::DeviceFactory::createUCM(&sp_, &ucm_);
		device_ptr_->start ();
	}
	response_codes_ = device_ptr_->querySuportDataLinkMessages().get();
	response_codes_ = device_ptr_->queryMaxPayload().get();
	response_codes_ = device_ptr_->querySuportIntermediateMessages().get();
	response_codes_ = device_ptr_->intermediateGetDeviceInformation().get();
	LOG(INFO) << "startup complete";

}  // end Constructor

ElectricWaterHeater::~ElectricWaterHeater () {
	delete device_ptr_;
}  // end Destructor

// Get Import Power
// -
unsigned int ElectricWaterHeater::GetImportPower () {
	return import_power_;
}  // end Get Import Power

// Get Import Energy
// -
unsigned int ElectricWaterHeater::GetImportEnergy () {
	return import_energy_;
}  // end Get Import Energy

// Set Import Watts
// -
void ElectricWaterHeater::SetImportWatts (unsigned int watts) {
	import_watts_ = watts;
}  // end Set Import Watts

// Update Commodity Data
// - 
void ElectricWaterHeater::UpdateCommodityData () {
	device_ptr_->intermediateGetCommodity ();
	std::vector <CommodityData> commodities = ucm_.GetCommodityData ();
	for (const auto &commodity : commodities) {
		if (commodity.code == 0) {
			import_power_ = commodity.rate;
		} else if (commodity.code == 7) {
			import_energy_ = commodity.cumulative;
		}
	}
}  // end Update Commodity Data

// Import Power
// - stop load shed to turn elements on and consume power from the grid
// - "Normal" operation means the elements should already be on if the
// - temperature is below the set-point
void ElectricWaterHeater::ImportPower () {
	if (import_watts_ > 0 
		&& import_energy_ > 0 
		&& ucm_.GetOpState () != OpState::NORMAL) {
		device_ptr_->basicEndShed (0);
	}
}  // end Import Power

// Export Power
// - export power is not possible for a water heater.
void ElectricWaterHeater::ExportPower () {
	// do nothing
}  // end Export Power

// Idle
// - shed load until either DERAS sends import command or temperature
// - falls below customer comfort levels.
void ElectricWaterHeater::Idle () {
	device_ptr_->basicShed(8);
}  // end Import Power
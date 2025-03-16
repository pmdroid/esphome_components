import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_DISTANCE,
    STATE_CLASS_MEASUREMENT,
    UNIT_PERCENT,
    ICON_WATER_PERCENT,
)

# We're using Wire library directly, but we need I2C pins
DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["sensor"]

CONF_I2C_LOW_ADDRESS = "i2c_low_address"
CONF_I2C_HIGH_ADDRESS = "i2c_high_address"

water_level_sensor_ns = cg.esphome_ns.namespace("water_level_sensor")
WaterLevelSensor = water_level_sensor_ns.class_(
    "WaterLevelSensor", sensor.Sensor, cg.PollingComponent
)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        WaterLevelSensor,
        unit_of_measurement=UNIT_PERCENT,
        icon=ICON_WATER_PERCENT,
        accuracy_decimals=0,
        device_class=DEVICE_CLASS_DISTANCE,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(cv.polling_component_schema("15s"))
    .extend({
        cv.Optional(CONF_I2C_LOW_ADDRESS, default=0x77): cv.hex_uint8_t,
        cv.Optional(CONF_I2C_HIGH_ADDRESS, default=0x78): cv.hex_uint8_t,
    })
)

async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    
    # Optionally set custom addresses
    if CONF_I2C_LOW_ADDRESS in config:
        cg.add(var.set_low_address(config[CONF_I2C_LOW_ADDRESS]))
    if CONF_I2C_HIGH_ADDRESS in config:
        cg.add(var.set_high_address(config[CONF_I2C_HIGH_ADDRESS]))
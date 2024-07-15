#include <voltage.h>
#include <heart.h>

float voltage() {
    adc1_config_width(ADC_WIDTH_BIT_12);  // 12-bit width
    adc1_config_channel_atten(VOLTAGE_PIN, ADC_ATTEN_DB_12); // Attenuation to handle higher range of voltage
    int adcValue = adc1_get_raw(VOLTAGE_PIN);
    float voltage = (adcValue / 4095.0) * 3.3;
    // Adjust for the voltage divider
    float actualVoltage = voltage * ((220.0 + 45.0) / 220.0);
    Serial.println(actualVoltage);
    return actualVoltage;

}
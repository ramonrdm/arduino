#include <VirtualWire.h>

char msg[500];
int n = 1;
void setup()
{
    Serial.begin(9600);	// Debugging only
    Serial.println("setup");
    vw_set_rx_pin(7);
    // Initialise the IO and ISR
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);	 // Bits per sec

    vw_rx_start();       // Start the receiver PLL running
}

void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
	int i;

        digitalWrite(13, true); // Flash a light to show received good message
	// Message with a good checksum received, dump it.
	Serial.print("Got: ");
        Serial.print(n);
        Serial.print(" ");
        n++;

	for (i = 0; i < buflen; i++)
	{
            msg[i]=buf[i];
	}
        msg[buflen]= '\0';
        Serial.println(msg);
        digitalWrite(13, false);
    }
}

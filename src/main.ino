// ===== Settings ===== //
const uint8_t channels[] = {1, 6, 11}; // used Wi-Fi channels (available: 1-14)
const bool wpa2 = true; // WPA2 networks
const bool appendSpaces = true; // makes all SSIDs 32 characters long to improve performance

/*
  SSIDs:
  - don't forget the \n at the end of each SSID!
  - max. 32 characters per SSID
  - don't add duplicates! You have to change one character at least
*/
const char ssids[] PROGMEM = {
    "Never Gonna Give You Up\n"
    "Never Gonna Let You Down\n"
    "Never Gonna Run Around\n"
    "And Desert You\n"
    "Never Gonna Make You Cry\n"
    "Never Gonna Say Goodbye\n"
    "Never Gonna Tell A Lie\n"
    "And Hurt You\n"
    "We've Known Each Other\n"
    "For So Long\n"
    "Your Heart's Been Actin'\n"
    "But You're Too Shy To Say\n"
    "Inside We Both Know\n"
    "What's Been Going On\n"
    "We Know The Game\n"
    "And We're Gonna Play It\n"
    "And If You Ask Me How I Feel\n"
    "Don't Tell Me You're Too Blind\n"
    "To See\n"
    "I Bet You Were Looking For This\n"
    "Is This What You're Searching For?\n"
    "Not Your Wi-Fi Sorry\n"
    "You Didn’t Think It’d Be That Easy Did You?\n"
    "I’m The Wi-Fi You Never Knew You Wanted\n"
    "Wi-Fi? I Barely Know Her\n"
    "Did You Find The Wi-Fi You Were Looking For?\n"
    "Look All You Want This Is Not Your Wi-Fi\n"
    "Searching... But For What?\n"
    "Your Connection is Unstable\n"
    "404 Wi-Fi Unavailable\n"
    "HideYourKidsHideYourWifi\n"
    "PrettyFlyForAWifi\n"
    "FBISurveillanceVan1\n"
    "CantStopTheSignal\n"
    "Believe ME This is Trusted Free wifi\n"
    "Abraham Linksy\n"
    "John Wilkes-Bluetooth\n"
    "99 Problems But WiFi Ain't One\n"
    "I’m Not a Witch I’m Your WiFi\n"
    "NSA Drone 3\n"
    "LAN BEFORE TIME\n"
    "Make Internet Great Again\n"
    "AllYourPacketsAreBelongToMe\n"
    "Lord of the Pings\n"
    "Still Loading…Please Wait\n"
    "1Kbps Speed (No One Will Try to Connect!)\n"
    "Get Your Own Damn WiFi\n"
    "Titanic Synching\n"
    "Disabled SSID\n"
    "The Silence of the LANs\n"
    "Click Here for Viruses\n"
    "Drop It Like It’s Hotspot\n"
    "I Pronounce You Man and WiFi\n"
    "That’s What She SSID\n"
    "I Did Your Wifi Last Night\n"
    "Happy Wifi Happy Lifi\n"
    "$10 Per Hour\n"
    "AOL Dialup\n"
    "Benjamin FrankLAN\n"
    "Theodore Routervelt\n"
    "Vladimir Routin\n"
    "Winona Router\n"
    "John Claude WAN Damme\n"
    "Iron LAN\n"
    "Spider LAN\n"
    "Virus Infected WiFi\n"
    "Virus Distribution Center\n"
    "This LAN Is Mine LAN None of Ur LAN\n"
    "Don’t $top Wi-Fi$hing\n"
    "Free WiFi. Just Kidding\n"
    "Enter Your ATM PIN for the Internet\n"
    "Your Music is Annoying\n"
    "Poor Signal\n"
    "No Facebook Happy Life\n"
    "WiFi Not Connected Try Again\n"
    "Uploading Your Photos 81% Completed\n"
    "Making Wi-Fi Great Once Again\n"
    "Mom Click Here for Internet\n"
    "Vladimir Computin\n"
    "One Does Not Simply Log Into Mordor\n"
    "WiMCA\n"
    "1000mbps All for Me Losers\n"
    "You Can’t Touch This\n"
    "GetOffMyLawn\n"
};
// 84 network


#include "WiFi.h"

extern "C" {
#include "esp_wifi.h"
  //typedef void (*freedom_outside_cb_t)(uint8 status);
  //int wifi_register_send_pkt_freedom_cb(freedom_outside_cb_t cb);
  //void wifi_unregister_send_pkt_freedom_cb(void);
  //int wifi_send_pkt_freedom(uint8 *buf, int len, bool sys_seq);
  esp_err_t esp_wifi_set_channel(uint8_t primary, wifi_second_chan_t second);
  esp_err_t esp_wifi_80211_tx(wifi_interface_t ifx, const void *buffer, int len, bool en_sys_seq);
}

// run-time variables
char emptySSID[32];
uint8_t channelIndex = 0;
uint8_t macAddr[6];
uint8_t wifi_channel = 1;
uint32_t currentTime = 0;
uint32_t packetSize = 0;
uint32_t packetCounter = 0;
uint32_t attackTime = 0;
uint32_t packetRateTime = 0;

// beacon frame definition
uint8_t beaconPacket[109] = {
  /*  0 - 3  */ 0x80, 0x00, 0x00, 0x00, // Type/Subtype: managment beacon frame
  /*  4 - 9  */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Destination: broadcast
  /* 10 - 15 */ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // Source
  /* 16 - 21 */ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // Source

  // Fixed parameters
  /* 22 - 23 */ 0x00, 0x00, // Fragment & sequence number (will be done by the SDK)
  /* 24 - 31 */ 0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00, // Timestamp
  /* 32 - 33 */ 0xe8, 0x03, // Interval: 0x64, 0x00 => every 100ms - 0xe8, 0x03 => every 1s
  /* 34 - 35 */ 0x31, 0x00, // capabilities Tnformation

  // Tagged parameters

  // SSID parameters
  /* 36 - 37 */ 0x00, 0x20, // Tag: Set SSID length, Tag length: 32
  /* 38 - 69 */ 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, // SSID

  // Supported Rates
  /* 70 - 71 */ 0x01, 0x08, // Tag: Supported Rates, Tag length: 8
  /* 72 */ 0x82, // 1(B)
  /* 73 */ 0x84, // 2(B)
  /* 74 */ 0x8b, // 5.5(B)
  /* 75 */ 0x96, // 11(B)
  /* 76 */ 0x24, // 18
  /* 77 */ 0x30, // 24
  /* 78 */ 0x48, // 36
  /* 79 */ 0x6c, // 54

  // Current Channel
  /* 80 - 81 */ 0x03, 0x01, // Channel set, length
  /* 82 */      0x01,       // Current Channel

  // RSN information
  /*  83 -  84 */ 0x30, 0x18,
  /*  85 -  86 */ 0x01, 0x00,
  /*  87 -  90 */ 0x00, 0x0f, 0xac, 0x02,
  /*  91 -  92 */ 0x02, 0x00,
  /*  93 - 100 */ 0x00, 0x0f, 0xac, 0x04, 0x00, 0x0f, 0xac, 0x04, /*Fix: changed 0x02(TKIP) to 0x04(CCMP) is default. WPA2 with TKIP not supported by many devices*/
  /* 101 - 102 */ 0x01, 0x00,
  /* 103 - 106 */ 0x00, 0x0f, 0xac, 0x02,
  /* 107 - 108 */ 0x00, 0x00
};

// goes to next channel
void nextChannel() {
  if (sizeof(channels) > 1) {
    uint8_t ch = channels[channelIndex];
    channelIndex++;
    if (channelIndex > sizeof(channels)) channelIndex = 0;

    if (ch != wifi_channel && ch >= 1 && ch <= 14) {
      wifi_channel = ch;
      //wifi_set_channel(wifi_channel);
      esp_wifi_set_channel(wifi_channel, WIFI_SECOND_CHAN_NONE);
    }
  }
}

// generates random MAC
void randomMac() {
  for (int i = 0; i < 6; i++)
    macAddr[i] = random(256);
}

void setup() {
  // create empty SSID
  for (int i = 0; i < 32; i++)
    emptySSID[i] = ' ';
  // for random generator
  randomSeed(1);

  // set packetSize
  packetSize = sizeof(beaconPacket);
  if (wpa2) {
    beaconPacket[34] = 0x31;
  } else {
    beaconPacket[34] = 0x21;
    packetSize -= 26;
  }

  // generate random mac address
  randomMac();
  
  // put your setup code here, to run once:
  Serial.begin(115200);
  //say hi
  Serial.println("Hello,NodeMCU!");
  //change WiFi mode
  WiFi.mode(WIFI_MODE_STA);

  // set channel
  esp_wifi_set_channel(channels[0], WIFI_SECOND_CHAN_NONE);

  // print out saved SSIDs
  Serial.println("SSIDs:");
  int i = 0;
  int len = sizeof(ssids);
  while(i < len){
    Serial.print((char)pgm_read_byte(ssids + i));
    i++;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  currentTime = millis();

  // send out SSIDs
  if (currentTime - attackTime > 100) {
    attackTime = currentTime;

    // temp variables
    int i = 0;
    int j = 0;
    int ssidNum = 1;
    char tmp;
    int ssidsLen = strlen_P(ssids);
    bool sent = false;

    // go to next channel
    nextChannel();

    while (i < ssidsLen) {
      // read out next SSID
      j = 0;
      do {
        tmp = pgm_read_byte(ssids + i + j);
        j++;
      } while (tmp != '\n' && j <= 32 && i + j < ssidsLen);

      uint8_t ssidLen = j - 1;

      // set MAC address
      macAddr[5] = ssidNum;
      ssidNum++;

      // write MAC address into beacon frame
      memcpy(&beaconPacket[10], macAddr, 6);
      memcpy(&beaconPacket[16], macAddr, 6);

      // reset SSID
      memcpy(&beaconPacket[38], emptySSID, 32);

      // write new SSID into beacon frame
      memcpy_P(&beaconPacket[38], &ssids[i], ssidLen);

      // set channel for beacon frame
      beaconPacket[82] = wifi_channel;

      // send packet
      if (appendSpaces) {
        for (int k = 0; k < 3; k++) {
          //packetCounter += wifi_send_pkt_freedom(beaconPacket, packetSize, 0) == 0;
          //Serial.printf("size: %d \n", packetSize);
          packetCounter += esp_wifi_80211_tx(WIFI_IF_STA, beaconPacket, packetSize, 0) == 0;
          delay(1);
        }
      }

      // remove spaces
      else {
        uint16_t tmpPacketSize = (109 - 32) + ssidLen; // calc size
        uint8_t* tmpPacket = new uint8_t[tmpPacketSize]; // create packet buffer
        memcpy(&tmpPacket[0], &beaconPacket[0], 37 + ssidLen); // copy first half of packet into buffer
        tmpPacket[37] = ssidLen; // update SSID length byte
        memcpy(&tmpPacket[38 + ssidLen], &beaconPacket[70], 39); // copy second half of packet into buffer

        // send packet
        for (int k = 0; k < 3; k++) {
          //packetCounter += wifi_send_pkt_freedom(tmpPacket, tmpPacketSize, 0) == 0;
          packetCounter += esp_wifi_80211_tx(WIFI_IF_STA, tmpPacket, tmpPacketSize, 0) == 0;
          delay(1);
        }

        delete tmpPacket; // free memory of allocated buffer
      }

      i += j;
    }
  }

  // show packet-rate each second
  if (currentTime - packetRateTime > 1000) {
    packetRateTime = currentTime;
    Serial.print("Packets/s: ");
    Serial.println(packetCounter);
    packetCounter = 0;
  }
}



#include <iostream>
#include <string>
#include <fcntl.h>     
#include <unistd.h>     
#include <linux/input.h> 
#include <string.h>
#include <cstdint>
#include <thread>
#include <atomic>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib> 

#define IS_DUALSENSE 1
#define IS_XBOX 0
#define PORT 4210 // ESP32 UDP server port 
#define IP "192.168.4.1" // ESP32 WiFi router IP  

#if IS_DUALSENSE
  #define PATH "Sony Interactive Entertainment DualSense Wireless Controller"
#endif

enum class PS5_BTNS: uint16_t {
  X = 304,
  TRIANGLE,
  SQUARE,
  CIRCLE
};

// if this is one set currentPress to the correct event key and send over udp 
std::atomic<int> buttonIsPressed{0};
std::atomic<int> currentPress{0};

// TODO: Add 2 seperate threads, one for udp client and one for processing inputs 

int FindDevice(void) {

  std::cout << "looking for device " << "\n";

  struct input_id device;
	char path[64];

	for(int i = 0; i < 64; i++) {
		sprintf(path,"/dev/input/event%d",i);
		int fd = open(path,O_RDONLY | O_NONBLOCK);
		if(fd >= 0){
			char buffer[256];

      std::size_t len = sizeof(buffer); 

			ioctl(fd,EVIOCGNAME(len),buffer);
  
			if(strcmp(buffer,PATH) == 0) {
        std::cout << "Found" << "\n";
				close(fd);
				return i;

			}

		}
		
	}

  std::cout << "Device was not found " << "\n";
	return -1;

}

// Create UDP Client for esp32 wifi connection
void createClient(void) {
  int s;

  char buffer[1024];

  struct sockaddr_in server_addr;
  std::size_t addr_len = sizeof(server_addr);


  int i = FindDevice();
  struct input_event event;

  char path[32];

  sprintf(path,"/dev/input/event%d",i);
  int fd = open(path, O_RDONLY | O_NONBLOCK);

  if(fd < 0) {
    std::cout << "Error opening file descriptor" << "\n";
    exit(0);
  }


  s = socket(AF_INET,SOCK_DGRAM,0);

  if(s < 0) {
    std::cout << "Could not create socket" << "\n";
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  
  if(inet_pton(AF_INET,IP,&server_addr.sin_addr) <= 0) {
    std::cout << "Wrong address" << "\n";
    exit(0);
  }

  const char* message = "hello";

  while(true) {
    int n = read(fd,&event,sizeof(event));

    if(n == sizeof(event)) {
      if(event.type == EV_KEY) {
        std::cout << "Button Pressed" << "\n";
          if(event.code == 304) {
            
            int bytes_sent = sendto(s,message,strlen(message),0,(struct sockaddr*)&server_addr,addr_len);
            std::cout << "Button pressed3" << "\n";
          }
      }
      //else if(event.type == EV_ABS) {
        //std::cout << "Joystick event" << "\n";
      //}
    }
  } 

  std::cout << "Packet Sent" << "\n";
  close(fd);
  close(s);
}

// Keep until i add different threads for the udp client and this function
void controller_loop(void) {

  int i = FindDevice();
  struct input_event event;

  char path[32];

  sprintf(path,"/dev/input/event%d",i);
  int fd = open(path, O_RDONLY | O_NONBLOCK);

  if(fd < 0) {
    std::cout << "Error opening file descriptor" << "\n";
    exit(0);
  }

  
  while(true) {
    int n = read(fd,&event,sizeof(event));

    if(n == sizeof(event)) {
      if(event.type == EV_KEY) {
        std::cout << "Button Pressed" << "\n";
          if(event.code == 304) {
            std::cout << "Button pressed3" << "\n";
          }
      }
      //else if(event.type == EV_ABS) {
        //std::cout << "Joystick event" << "\n";
      //}
    }
  } 
  close(fd);
}

int main(void) {
  createClient();
  
}

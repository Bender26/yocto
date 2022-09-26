#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <memory>
#include <iostream>
class i2cWrapper{
    public:
    i2cWrapper(){
        if ((i2cAdress = open(devInLinux, O_RDWR)) < 0)
        {
            std::cout << "Failed to open the bus.\n";
            exit(1);
        }
        std::cout << "Requested I2C is available\n";
        if (ioctl(i2cAdress, I2C_SLAVE, devAddr) < 0) {
            std::cout << "Failed to set up the bus or device.\n";
            exit(1);
        }
        std::cout << "I2C wrapper is ready\n";
    }
    
    bool writeI2C(uint8_t* buffer) {
        if (write(i2cAdress, buffer, 1) != 1)
        {
            std::cout << "Failed to write into I2C.\n";
            return false;
        }
        return true;
    }
    bool readI2C(uint8_t* buffer) {
        if (read(i2cAdress, buffer, 2) != 2)
        {
            std::cout << "Failed to read from I2C.\n";
            return false;
        }    
        return true;  
    } 
    private:
    int i2cAdress{0};
    char devInLinux[11] = "/dev/i2c-1";
    uint8_t devAddr = 0x48;
    uint8_t memAddr = 0x00;
};

class getTemp{
    public:
    getTemp(){
        std::cout << "Starting app\n";
        i2cDevice = std::make_unique<i2cWrapper>();
        if(!i2cDevice){
            std::cout << "Failed to create i2c RAII wrapper\n";
            exit(1);
        } 
    }

    void measure(){
        buffer[0] = 0x00;
        read();
        calculate();
        printValue();
    }

    private:
    void read(){
        if(!i2cDevice->writeI2C(buffer)) exit(1);
        if(!i2cDevice->readI2C(buffer)) exit(1);
    }
    void calculate() {
        tempBuffer = (buffer[0] << 4) | (buffer[1] >> 4);
        if (tempBuffer > 0x7FF)
        {
            tempBuffer |= 0xF000;
        }
        temp = tempBuffer * 0.0625;
    }
    void printValue() {
        std::cout << "Measured: " << temp << " degrees of Celcius." << std::endl;
    }
    uint8_t buffer[5];
    std::unique_ptr<i2cWrapper> i2cDevice;
    float temp;
    int16_t tempBuffer;
};

int main()
{
    getTemp worker;
    worker.measure();
        
    return 0;
}
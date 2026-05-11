#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <list>
#include <array>
#include <variant>
#include <typeinfo>
#include <random> // Essential header



struct csvData{};

using double_str = std::variant<std::string , double>;

struct DataContainer{

    DataContainer(const std::string& filename,int headerlength = 0){
        std::ifstream file(filename);
        std::string line;
        int colNumb {0};
        // std::vector<double_str> row;

        // Check if the file is open
        if (!file.is_open()) {
            std::cerr << "Could not open the file!" << std::endl;
            return ;
        }
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string value;
            
            std::vector<double_str> temp; 
            // Split the line by commas
            while (std::getline(ss, value, ',')) {
                if(colNumb+1 < headerlength){
                    this->headerValues[colNumb++] = value;
                }
                // not able to push to row for some reason ** fix this later
                else{
                    if(isNumber(value))
                        temp.push_back(std::stod(value));
                    else
                        temp.push_back(value);
                    
                    if(temp.size() == headerlength){
                        dataValues.push_back(temp);
                        temp.clear();
                    }
                }
                
            }
        }
        // this is just to test if we are getting the data correctly ** remove this later
        for(auto& field : dataValues){
            for(auto& fieldTwo : field){
                std::visit([](auto&& arg){
                    std::cout << arg << ' ';
                }, fieldTwo);
            }
            std::cout << '\n';
        }
        
    }

    double_str getData(int indexI , int IndexJ){
        return this->dataValues[indexI][IndexJ];
    }

    void getInnerValues(){
        for( const auto& field : this->headerValues){
            std::cout << field << '\n';
        }
    }

    private:
        // fasted option to access . All in stack
        std::array<std::string,15> headerValues{"", "", "", "", "", "", "", "", "", "","","","","",""}; 
        // using dataValue = std::variant<std::string , double>;
        using headerFields = std::vector<
            std::vector<
                std::string
            >
        >;
        headerFields headerFieldValues;

        std::vector<std::vector<double_str>> dataValues;
        // we store all the data values in this

        bool isNumber(const std::string& s) {
            try {
                std::stod(s);
                return true;
            } catch (...) {
                return false;
            }
        }




};

struct MlModel {


    void getData(){
        // get data for model
    }

    void splitData(){
        // split data from model
    }
    
    void Fit(){
        // train model
    }

    void Predict(){
        // predict model
    }

    void predictError(){
        // MSE or something 
    }
};


struct LinerRegression:MlModel{

    private:
        // model parameters
        double alpha {0.01}; // learning rate
        int iterations {1000};
        std::vector<double> weights;
        double bias {0.0};

        void getInitialWeights(const int& featureNumb ){
            // get initial weights
            for(int i = 0; i < featureNumb; i++){
                this->weights.push_back(0.0);
            }
        }

        int shuffler {0};

        bool checkRandomSeed(int& index , int& seed , int& dataSize , int& percentage , bool isTrain = true){
                // we use random seed formula to shuffle data and get random samples for training and testing
            return isTrain == ((index * shuffler + seed ) % dataSize < percentage);
        }

    public:

        LinerRegression(std::vector<double>){
            this->getInitialWeights(22);
            // get data size
            // add 55 + data_size to the shuffler number
        }

        void Fit(){
            // train model
            // y = w1*x1 + w2*x2 + ... + wn*xn + b
            

        }

        void Predict(){
            // predict model
        }

         void predictError(){
            // MSE or something 
        }
        ~LinerRegression(){
            // destructor
        }
};



int main() {

    DataContainer dataContainer{"../data/regressionData/house.csv" , 8};
    return 0;
}
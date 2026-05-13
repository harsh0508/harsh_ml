#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <list>
#include <array>
#include <variant>
#include <typeinfo>
#include <random>



// task : replace all the double_str with DataCell and make necessary changes to the code to work with DataCell instead of double_str

struct csvData{};

using double_str = std::variant<std::string , double>;


struct DataCell{

    std::variant<std::string , double> value;  //data type we are using

    DataCell(double val) : value(val) {} // constructor for double
    DataCell(const std::string& val) : value(val) {} // constructor for string
    DataCell(const char* val) : value(std::string(val)) {} // for string literals

    double toDouble()const{ // converts data to double if possible
        if(auto d = std::get_if<double>(&value)){
            return *d;
        }
        if(auto s = std::get_if<std::string>(&value)){
            try {
                return std::stod(*s);
            } catch (...) {
                throw std::runtime_error("Cannot convert string to double");
            }
        }
        throw std::runtime_error("Invalid data type");

    }

    std::string toString() const{ // converts data to string if possible
        if(auto s = std::get_if<std::string>(&value)){
            return *s;
        }
        if(auto d = std::get_if<double>(&value)){
            return std::to_string(*d);
        }
        return "";

    }

    friend std::ostream& operator<<(std::ostream& os, const DataCell& cell) { // overloading operator for printing data
        std::visit(
            [&os](auto&& arg) { // lambda function to print data
                os << arg;
            }           
            ,cell.value);
        return os;
    }

};

struct DataContainer{


    void printData(std::vector<std::vector<double_str>> dataValues){
        
        for(auto& field : dataValues){
            for(auto& fieldTwo : field){
                std::visit([](auto&& arg){
                    std::cout << arg << ' ';
                }, fieldTwo);
            }
            std::cout << '\n';
        }
    }

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
        // add or remove to print data **
        // printData(this->dataValues);
        
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
        // store header values
        std::array<std::string,15> headerValues{"", "", "", "", "", "", "", "", "", "","","","","",""}; 
        // data storage below
        std::vector<std::vector<double_str>> dataValues;
       
        // checks if data type is number i.e double or string
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
        int epochs {1000}; // number of iteration / epochs
        std::vector<double> weights; // weights for each feature
        double bias {0.0}; // bias term
        int shuffler {0}; // random seed shuffler

        std::vector<double_str> dataValues; // data values for model
        int headerLeangth {0}; // header length for data

        std::vector<std::vector<double_str>> normalizedData ; // normalized data for model
        std::vector<std::tuple<double , double>> minMaxValues; // to store min and max values for each feature


        void getInitialWeights(const int& featureNumb ){
            // get initial weights
            for(int i = 0; i < featureNumb; i++){
                this->weights.push_back(0.0);
            }
        }
        bool checkRandomSeed(int& index , int& seed , int& dataSize , int& percentage , bool isTrain = true){
                // we use random seed formula to shuffle data and get random samples for training and testing
            return isTrain == ((index * shuffler + seed ) % dataSize < percentage);
        }

        std::vector<std::vector<double_str>> getNormalizedData(std::vector<std::vector<double_str>>& dataValues , int& headerLeangth){
            // normalize data using min-max normalization
        
            // get min max for each feature
            double min;
            double max;
            int index = {0};
            for(int i = 0; i < headerLeangth; i++){
                int j = {0};
                min = dataValues[i][index];
                max = dataValues[i][index];
                while(j < dataValues.size()){
                    if(dataValues[j][index] < min)
                        min = dataValues[j][index];
                    if(dataValues[j][index] > max)
                        max = dataValues[j][index];
                    j++;
                }
                index++;
                minMaxValues.push_back(std::make_tuple(min, max));
            }

            // normalize data using min max values
            for(auto& row : dataValues){
                std::vector<double_str> temp={};
                int index = {0};
                for(auto& column : row){
                    // normalize here **
                    // normalized_value = (value - min) / (max - min)
                     double value = column;
                     temp.push_back((value - std::get<0>(minMaxValues[index])) / (std::get<1>(minMaxValues[index]) - std::get<0>(minMaxValues[index])));
                     index++;
                }
                normalizedData.push_back(temp);
            }

            
            return normalizedData;
        }

    public:

        LinerRegression(std::vector<double_str> dataValues , int headerLeangth){
            this->dataValues = dataValues;
            this->headerLeangth = headerLeangth;
            this->getInitialWeights(22);
            
            this->shuffler = 55 + this->dataValues.size(); // add 55 + data_size to the shuffler number
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
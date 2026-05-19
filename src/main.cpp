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



// task : check how seed is working and implement it with shuffling data and splitting it into training and testing data

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

    DataContainer(const std::string& filename,int headerlength = 0){
        std::ifstream file(filename);
        std::string line;
        int colNumb {0};

        // Check if the file is open
        if (!file.is_open()) {
            std::cerr << "Could not open the file!" << std::endl;
            return ;
        }
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string value;
            
            std::vector<DataCell> temp; 
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
        
    }

    std::vector<std::vector<DataCell>> getData(){
        return this->dataValues;
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
        std::vector<std::vector<DataCell>> dataValues;
       
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
        int epochs ; // number of iteration / epochs
        std::vector<double> weights; // weights for each feature
        double bias {0.0}; // bias term
        int shuffler {0}; // random seed shuffler
        int trainingPercentage {0}; // percentage of data used for training
        int seed {0}; // random seed for shuffling data

        std::vector<std::vector<DataCell>> dataValues; // data values for model
        int headerLeangth {0}; // header length for data

        std::vector<std::vector<DataCell>> normalizedData ; // normalized data for model
        std::vector<std::tuple<double , double>> minMaxValues; // to store min and max values for each feature


        void getInitialWeights(const int& featureNumb ){
            // get initial weights
            for(int i = 0; i < featureNumb; i++){
                this->weights.push_back(0.0);
            }
        }
        bool checkRandomSeed(int& index , int& seed ,const int& dataSize ,const int& percentage , bool isTrain = true){
                // we use random seed formula to shuffle data and get random samples for training and testing
            return isTrain == ((index * shuffler + seed ) % dataSize < dataSize * percentage / 100); // if isTrain is true, we get training data, else we get testing data
        }

        void normalizeData(){
            // normalize data using min-max normalization
        
            // get min max for each feature
            double min;
            double max;
            int index = {0};
            for(int i = 0; i < headerLeangth; i++){
                int j = {0};
                min = dataValues[0][index].toDouble();
                max = dataValues[0][index].toDouble();
                while(j < dataValues.size()){
                    if(dataValues[j][index].toDouble() < min)
                        min = dataValues[j][index].toDouble();
                    if(dataValues[j][index].toDouble() > max)
                        max = dataValues[j][index].toDouble();
                    j++;
                }
                index++;
                minMaxValues.push_back(std::make_tuple(min, max));
            }

            // normalize data using min max values
            for(auto& row : dataValues){
                std::vector<DataCell> temp={};
                int index = {0};
                for(auto& column : row){
                    // normalize here **
                    // normalized_value = (value - min) / (max - min)
                     double value = column.toDouble();
                     temp.push_back((value - std::get<0>(minMaxValues[index])) / (std::get<1>(minMaxValues[index]) - std::get<0>(minMaxValues[index])));
                     index++;
                }
                normalizedData.push_back(temp);
            }

            
            return ;
        }

        void rescaleData(){

        }

    public:

        LinerRegression(std::vector<std::vector<DataCell>> dataValues , int headerLeangth){
            this->dataValues = dataValues;
            this->headerLeangth = headerLeangth;
            normalizeData(); // normalize data before training
            this->getInitialWeights(headerLeangth-1); //
            
            this->shuffler = 55 + this->dataValues.size(); // add 55 + data_size to the shuffler number
        }

        void Fit(int epochs = 100 , bool earlyStopping = false , int percentage = 80 , int seed = 0){
            // ** implenet random seed
            // **implement early stopping later
            // train model
            // y = w1*x1 + w2*x2 + ... + wn*xn + b
            this->trainingPercentage = percentage;
            this->seed = seed;
            double recordedError {}; // to record error for early stopping

            for(int e = 0; e < epochs; e++){
                int rowIndex = {0};
                for(auto &row : normalizedData){
                    if(checkRandomSeed(rowIndex , seed , normalizedData.size() , percentage)){ // specified percentage for training
                        double y_pred = bias; // initialize predicted value with bias
                        for(int i = 0; i < headerLeangth-1; i++){
                            y_pred += weights[i] * row[i].toDouble(); // calculate predicted value : y_pred = w1*x1 + w2*x2 + ... + wn*xn + b
                        }
                        double error = row[headerLeangth-1].toDouble() - y_pred; // calculate error : error = y_true - y_pred
                        // std::cout << "Epoch: " << e+1 << " Error: " << error << '\n'; // print error for each epoch
                        bias += alpha * error; // update bias
                        for(int i = 0; i < headerLeangth-1; i++){
                            weights[i] += alpha * error * row[i].toDouble(); // update weights
                        }
                    }
                    rowIndex++;
                }
                // if(e % 100 == 0){
                    // ** implement MSE for early stopping
                    // std::cout << "Epoch: " << e+1  << ",Mean Squared Error: " <<   << '\n';
                // }
            }
        }

        void Predict(){
            // predict model
            int rowIndex = {0};
            for(const auto& row : normalizedData){
                if(checkRandomSeed(rowIndex, seed , normalizedData.size() , trainingPercentage , false)){ // specified percentage for testing
                    double y_pred = bias; // initialize predicted value with bias
                    for(int i = 0; i < headerLeangth-1; i++){
                        y_pred += weights[i] * row[i].toDouble(); // calculate predicted value : y_pred = w1*x1 + w2*x2 + ... + wn*xn + b
                    }
                    std::cout << "Predicted Value: " << y_pred <<" ,Original: " << row[headerLeangth-1].toDouble() <<", Error: " << row[headerLeangth-1].toDouble() - y_pred << '\n'; // print predicted value
                }
                rowIndex++;
            }
        }
        ~LinerRegression(){
            // destructor
        }
};



int main() {

    int HEADER_LENGTH = 8; // header length for data
    DataContainer dataContainer{"../data/regressionData/house.csv" , HEADER_LENGTH};
    LinerRegression linerModel(dataContainer.getData() , HEADER_LENGTH);
    linerModel.Fit(5000, false, 80, 42); // train model

    linerModel.Predict(); // predict model

    return 0;
}
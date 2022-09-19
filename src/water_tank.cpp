class WaterTank{
    private:
        const char * _id;
        unsigned int capacity = 0;
        float value = 0;
        
    public:
        WaterTank(const char * id);
        bool enabled = false; 
        inline unsigned int getCapacity(){
            return capacity;
        }
        inline float getValue(){
            return value;
        }
        void reset();
};

WaterTank::WaterTank(const char * id){
    this->_id = id;
}

void WaterTank::reset(){
    value = capacity;
}
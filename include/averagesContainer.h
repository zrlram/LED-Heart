struct averagesContainer{

    uint16_t *samples; //create pointer to samples for memory consideration
    uint16_t container_size;
    uint8_t counter;
    uint16_t minLvl;
    uint16_t maxLvl;

    //Constructor (Class Object Initializer)
    averagesContainer(uint16_t datapoints) {
        counter = 0;
        container_size = datapoints;
        samples = (uint16_t*) malloc(sizeof(uint16_t)*container_size); //Creates
    }

    //Define class functions for each object -> allows to generate averages, etc.
    bool setSample(uint16_t value) {
        if(counter < container_size) { //if we haven't hit max size of container
            samples[counter++] = value; //Save the value we call function with
            return true; //true means allow to continue adding
        } else { //Container is full. Reset counter to allow roll-over/rewriting of container
            counter = 0;
            return false; //Flag that container is full
        }
    }

    int findAverage(){
        int sum = 0;
        for (int i = 0; i<container_size; i++){ //Loop through container, add values
            sum += samples[i];
        }
        return (int)(sum/container_size);
    }
    void MinMax(){
        minLvl = maxLvl = samples[0];
        for(int i = 1; i<container_size; i++){
            if(samples[i]<minLvl) minLvl = samples[i];
            else if(samples[i]>maxLvl) maxLvl = samples[i];
        }
    }
    uint16_t getMin(){
        return minLvl;
    }
    uint16_t getMax() {
        return maxLvl; 
    }
};
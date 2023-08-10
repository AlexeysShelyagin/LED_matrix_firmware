#include "encoder.h"

Encoder_data::Encoder_data(int turns_, int clicks_){
    turns = turns_;
    clicks = clicks_;
}

Encoder::Encoder(int sa_pin_, int sb_pin_, int sw_pin_){
    sa_pin = sa_pin_;
    sb_pin = sb_pin_;
    sw_pin = sw_pin_;

    buff_size = 0;
}

int Encoder::get_rotation(bool v1, bool v2){
    if(v1 == 1 && v2 == 1) 
        ready_to_read = true;

    else if(ready_to_read){
        ready_to_read = false;
        return (v1 == 1) ? 1 : -1;
    }
    
    return 0;
}

bool Encoder::get_click(bool val){
    if(unpressed && !val){
        unpressed = false;
        return true;
    }

    if(val)
        unpressed = true;
    
    return false;
}

void Encoder::tick(){
    if(buff_size >= MAX_BUFFER_SIZE) return;
    
    buffer[buff_size][0] = digitalRead(sa_pin);
    buffer[buff_size][1] = digitalRead(sb_pin);
    buffer[buff_size][2] = digitalRead(sw_pin);
    buff_size++;
};

Encoder_data Encoder::get_updates(){
    Encoder_data response;
    for(int i = 0; i < buff_size; i++){
        response.turns += get_rotation(buffer[i][0], buffer[i][1]);
        response.clicks += (int) get_click(buffer[i][2]);
    }
    buff_size = 0;

    return response;
}
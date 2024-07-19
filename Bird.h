#include <SFML/Graphics.hpp>
class Bird{

    private:
    sf::Texture textures[2];
    const int gravity;
    const int flap_speed ;
    float velocity_y;
    //Anim_counter to control the flapping duration of bird.
    int anim_counter, texture_switch;
    bool should_fly;

    public:
    sf::Sprite bird_sprite;    //Making public to access from outside
    Bird();
    void setShouldFly(bool);
    void flapBird(sf::Time& dt);  // is called for flapping the bird
    void resetBirdPosition();
    void update(sf::Time& dt);  
    float getRightBound();  //to get right hand side x-coordinate of bird

};
/////////////////////////////////////////////////////////////////
/*!

* \File Name: Vehicles.cpp

* \author: Wong Keng Han Ashley

* \date: 14 feb 2016

* \description: functions and data for all vehicles

*/
/////////////////////////////////////////////////////////////////

#include "Vehicles.h"

/////////////////////////////////////////////////////////////////
/*!

* \method: constructor

* \author: Wong Keng Han Ashley

* \date: 16 feb 2016

* \description: constructor for vehicals

*/
/////////////////////////////////////////////////////////////////
Vehicles::Vehicles() :
Yaw(0),
delay(0),
board(false),
isDead(false),
bulletCooldown(0)
{




}

/////////////////////////////////////////////////////////////////
/*!

* \method: overloaded constructor

* \author: Wong Keng Han Ashley

* \date: 16 feb 2016

* \description: constructor for vehicals

*/
/////////////////////////////////////////////////////////////////
Vehicles::Vehicles(Vector3 moveDirection) :
Yaw(0),
delay(0),
board(false),
isDead(false),
bulletCooldown(0)
{
	
}

/////////////////////////////////////////////////////////////////
/*!

* \method: overloaded constructor

* \author: Wong Keng Han Ashley

* \date: 16 feb 2016

* \description: overloaded constructor for vehicals

*/
/////////////////////////////////////////////////////////////////
Vehicles::Vehicles(Vector3 position, Vector3 viewDirection, float newSpeed, int newHealth, float newFireRate, float newBulletDamage) :
board(false),
isDead(false),
bulletCooldown(0)
{
    Vector3 initialPosition = position + viewDirection.Normalize();
    Pos = position;
    View = viewDirection;
    newVehicle.setSpeed(newSpeed);
    newVehicle.setCurrentLocation(position);

    initialYaw = getRotationAngle(viewDirection);
    currAttackTarget = nullptr;
    maxHealth = newHealth;
	health = maxHealth;
    bulletFireRate = newFireRate;
    bulletDamage = newBulletDamage;
}

/////////////////////////////////////////////////////////////////
/*!

* \method: destructor

* \author: Wong Keng Han Ashley

* \date: 16 feb 2016

* \description: destructor for vehicals

*/
/////////////////////////////////////////////////////////////////
Vehicles::~Vehicles(){

}

/////////////////////////////////////////////////////////////////
/*!

* \method: update

* \author: Wong Keng Han Ashley

* \date: 16 feb 2016

* \description: update the vehicle

*/
/////////////////////////////////////////////////////////////////
void Vehicles::update(double dt){


    if (health <= 0){

        isDead = true;

    }

    if (!isDead){

        newVehicle.pathRoute(dt);
        Pos = newVehicle.getCurrentLocation();

		updateHitbox();

        SetInteraction(AABB(Vector3(Pos.x - InteractionMin.x, Pos.y - InteractionMin.y, Pos.z - InteractionMin.z), Vector3(Pos.x + InteractionMax.x, Pos.y + InteractionMax.y, Pos.z + InteractionMax.z)));
        Yaw = getRotationAngle();

        if (currAttackTarget != nullptr){

            newVehicle.updateWayPoints(currAttackTarget->Pos);

        }


        Pos.y = 0;

    }

    bulletCurrCooldown += dt;


}

/////////////////////////////////////////////////////////////////
/*!

* \method: setNewWayPoint

* \author: Wong Keng Han Ashley

* \date: 16 feb 2016

* \description: setting waypoints  for the object

*/
/////////////////////////////////////////////////////////////////
void Vehicles::setNewWayPoint(float x, float z){

        newVehicle.updateWayPoints(Vector3(x, 0, z));

}

/////////////////////////////////////////////////////////////////
/*!

* \method: initialMoveDirection

* \author: Wong Keng Han Ashley

* \date: 16 feb 2016

* \description: setting Initial Move Direction  for the object

*/
/////////////////////////////////////////////////////////////////
void Vehicles::initialMoveDirection(float x, float z){

	newVehicle.setInitialWayPoints(Vector3(x, 0, z));

}


/////////////////////////////////////////////////////////////////
/*!

* \method: overloaded initialMoveDirection

* \author: Wong Keng Han Ashley

* \date: 16 feb 2016

* \description: setting Initial Move Direction  for the object

*/
/////////////////////////////////////////////////////////////////
void Vehicles::initialMoveDirection(){

	newVehicle.setInitialWayPoints(Pos, View);

}
 

/////////////////////////////////////////////////////////////////
/*!

* \method: getRotationAngle

* \author: Wong Keng Han Ashley

* \date: 18 feb 2016

* \description: setting initial waypoints for the object

*/
/////////////////////////////////////////////////////////////////
float Vehicles::getRotationAngle(){


    float degree = Math::RadianToDegree(acos(View.Dot(newVehicle.getLastWayPointDirection())));

    
    Vector3 n = View.Cross(newVehicle.getLastWayPointDirection());
         
    if ((n.Dot(Vector3(0, 1, 0))) < 0){

        degree *= -1;

    }
   


    return initialYaw + degree;

}


/////////////////////////////////////////////////////////////////
/*!

* \method: setThrust

* \author: Wong Keng Han Ashley

* \date: 18 feb 2016

* \description: sets the speed of the vehicle

*/
/////////////////////////////////////////////////////////////////
void Vehicles::setThrust(float newThrust){

    newVehicle.setSpeed(newThrust);

}

/////////////////////////////////////////////////////////////////
/*!

* \method: overloaded getRotationAngle

* \author: Wong Keng Han Ashley

* \date: 18 feb 2016

* \description: setting initial waypoints for the object

*/
/////////////////////////////////////////////////////////////////
float Vehicles::getRotationAngle(Vector3 newView){

	Vector3 initView(0, 0, 1);
    float degree = Math::RadianToDegree(acos(initView.Dot(View)));

    //Vector3 n = View.Cross(newVehicle.getLastWayPointDirection());
	Vector3 n = initView.Cross(newView);

    if ((n.Dot(Vector3(0, 1, 0))) < 0){

        degree *= -1;

    }

   
    return degree;
}

/////////////////////////////////////////////////////////////////
/*!

* \method: fire bullets

* \author: Wong Keng Han Ashley

* \date: 23 feb 2016

* \description: returns if the target is in range

*/
/////////////////////////////////////////////////////////////////
bool Vehicles::fireBullets(double dt){

    if (currAttackTarget != nullptr){

        if (currAttackTarget->Pos.distanceBetween2points(Pos) < 400){

            if (bulletFireRate > 0 && bulletCurrCooldown > 100 / static_cast<float>(bulletFireRate)){

                bulletCurrCooldown = 0;
                return true;
            }

        }

    }

    return false;

}




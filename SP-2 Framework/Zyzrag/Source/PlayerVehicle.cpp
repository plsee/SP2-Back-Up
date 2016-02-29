/******************************************************************************/
/*!
\file	PlayerVehicle.cpp
\author Goh Zheng Yuan
\par	email: 150377B@mymail.nyp.edu.sg
\brief
Contains anything related to player's vehicle
*/
/******************************************************************************/
#include "PlayerVehicle.h"

/******************************************************************************/
/*!
\brief
Update player vehicle

\param dt 
	Delta Time

\param hitbox
	Vector of AABB hitbox, to test if vehicle is colliding with anything
*/
/******************************************************************************/
void PlayerVehicle::update(double dt, vector<AABB> hitbox){

    bulletCurrCooldown += dt;
	if (health <= 0)
	{
		thrust = 0;
		Pos.SetZero();
		isDead = true;
	}
		
	if (isDead == true)
		respawnTimer += dt;

	if (thrust != 0)
	{
		bool checkX, checkY, checkZ;
		checkX = checkY = checkZ = false;
		for (int i = 0; i < hitbox.size(); ++i)
		{
			if (this->hitbox.AABBtoAABB(hitbox[i], Vector3(View.x * dt * thrust, 0, 0)) && checkX == false)
			{
				checkX = true;
			}
			if (this->hitbox.AABBtoAABB(hitbox[i], Vector3(0, View.y * dt * thrust, 0)) && checkY == false)
			{
				checkY = true;
			}
			if (this->hitbox.AABBtoAABB(hitbox[i], Vector3(0, 0, View.z * dt * thrust)) && checkZ == false)
			{
				checkZ = true;
			}
		}
		if (!checkX)
		{
			Pos.x += View.x * dt * thrust;
		}
		if (!checkY)
		{
			Pos.y += View.y * dt * thrust;
		}
		if (!checkZ)
		{
			Pos.z += View.z * dt * thrust;
		}

		if (checkX || checkY || checkZ)
		{
			if (thrust > 30)
			{
				health -= thrust;
				thrust *= -1;
				if (thrust < -40)
					thrust = -40;
			}
			
		}
		updateHitbox();
	}
}

/******************************************************************************/
/*!
\brief
Respawn timer

\param time
	How much time it should take before respawning.

\return	
	True when respawnTimer hits the time given, else false
*/
/******************************************************************************/
bool PlayerVehicle::respawn(int time)
{
	if (respawnTimer > time)
	{
		respawnTimer = 0;
		isDead = false;
		return true;
	}
	return false;
}

/******************************************************************************/
/*!
\brief
Resets everything to default values
*/
/******************************************************************************/
void PlayerVehicle::reset()
{
	thrust = 0;
	yaw = 0;
	pitch = 0;
	health = maxHealth;
}

/******************************************************************************/
/*!
\brief
	Function use to see if bullets are ready to be fired

\param bulletFireRate
	Pass in the bullet's fire rate to the function

\return
	True when bullet cooldown is more then fire rate. else false 
*/
/******************************************************************************/
bool PlayerVehicle::fireBullets(int bulletFireRate){

    float currFireRate = 100 / static_cast<float>(bulletFireRate);

    if (bulletFireRate > 0 && bulletCurrCooldown > currFireRate){
    
         bulletCurrCooldown = 0;

         return true;

     }

    return false;

}

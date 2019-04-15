#include<vector>
#include<iostream>
#include<fstream>
#include<cmath>
#include<iomanip>

struct planet {
  double x;
  double y;
  double ux;
  double uy;
  double ax;
  double ay;
  double m;
  double r;
};




// m = density * Pi * r ^ 2
// m * a = f
// f = G * m1 * m2 / r ^ 2
class Space {
  private:
    std::vector<planet> planet_system;
//  	const double g = 6.67 * 10e-11;
    const double g = 1;
    double dt = 0.01;
    int iterations_number = 0;
    const bool key_elastic_collision = true;
    
    // 0 = no collisions (point unit setup)
    // 1 = elastic collision
//    const int collision_type = 0;
    


    double dist(planet p1, planet p2) {
      return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
    }

    void calculate_acceleration() {
      for (int i = 0; i < planet_system.size(); ++i) {
        planet_system[i].ax = 0;
        planet_system[i].ay = 0;
        for (int j = 0; j < planet_system.size(); ++j) {
          double dist_ij = dist(planet_system[i], planet_system[j]);
          if (dist_ij > 0) {
            planet_system[i].ax += g * planet_system[j].m * (planet_system[j].x - planet_system[i].x) / std::pow(dist_ij, 3);
            planet_system[i].ay += g * planet_system[j].m * (planet_system[j].y - planet_system[i].y) / std::pow(dist_ij, 3);
          }
        }
      }
    }

    void calculate_velocity() {
      for (int i = 0; i < planet_system.size(); ++i) {
        planet_system[i].ux += planet_system[i].ax * dt;
        planet_system[i].uy += planet_system[i].ay * dt;
      }
    }

    void calculate_position() {
      for (int i = 0; i < planet_system.size(); ++i) {
        planet_system[i].x += planet_system[i].ux * dt;
        planet_system[i].y += planet_system[i].uy * dt;
      }
    }
    
    void calculate_elastic_collision() {
    	return;
		}

    void make_iteration() {
      calculate_acceleration();
      calculate_velocity();
      calculate_position();
    }
    
    std::ostream& write_iteration(std::ostream& result, int n_iter) {
			int prec = 12;
			int l = 6;
      result.precision(prec);
      result.setf(std::ios::fixed);

		  result << "Zone T = " << '"' << n_iter << '"' << std::endl;
      for (int i = 0; i < planet_system.size(); ++i) {
        result << "  " << std::right << std::setw(prec + l) << planet_system[i].x
               << "  " << std::right << std::setw(prec + l) << planet_system[i].y
               << "  " << std::right << std::setw(prec + l) << planet_system[i].ux
               << "  " << std::right << std::setw(prec + l) << planet_system[i].uy
               << "  " << std::right << std::setw(prec + l) << planet_system[i].ax
               << "  " << std::right << std::setw(prec + l) << planet_system[i].ay
               << "  " << std::right << std::setw(prec + l) << planet_system[i].m
               << "  " << std::right << std::setw(prec + l) << planet_system[i].r << std::endl;
      }
     	return result;
		}

  public:
    Space() {
      std::ifstream input("setup.dat");
      int planets_number = 0;
      
      
      input >> iterations_number;
      input >> dt;

      input >> planets_number;

      for (int i = 0; i < planets_number; ++i) {
        planet inp_planet;
        input >> inp_planet.m;
        input >> inp_planet.x;
        input >> inp_planet.y;
        input >> inp_planet.ux;
        input >> inp_planet.uy;
//        inp_planet.ux = 0;
//        inp_planet.uy = 0;
        inp_planet.ax = 0;
        inp_planet.ay = 0;
        inp_planet.r = std::sqrt(inp_planet.m / 3.1415926);
        planet_system.push_back(inp_planet);
      }
      input.close();
    }

    void solve(int n_iter) {
      std::ofstream history("history.tec");
      history << "Variables = x, y, ux, uy, ax, ay, m, r" << std::endl;
   		write_iteration(history, 0);
    	for (int i = 0; i < n_iter; ++i) {
    		make_iteration();
    		write_iteration(history, i + 1);
			}
      history.close();    		
		}
    
    

};



int main()
{
  Space my_space;
//  my_space.write_Position("t1.tec");
  my_space.solve(2000);
//  my_space.write_Position("t2.tec");

  return 0;
}


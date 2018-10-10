// Generated by dia2code
#ifndef STATE__CHARACTERS__H
#define STATE__CHARACTERS__H

#include <string>

namespace state {
  class Statistics;
  class Position;
}

#include "Statistics.h"
#include "Position.h"

namespace state {

  /// class Characters - 
  class Characters {
    // Associations
    // Attributes
  public:
    std::string name;
    Statistics stats;
    Position position;
    int attack_cost [number_of_attacks];
    int attack_damage [number_of_attacks];
    int attack_field_of_action [number_of_attacks];
    // Operations
  public:
    Characters ();
    Characters (Statistics stats);
    Characters (std::string name);
    // Setters and Getters
  };

};

#endif

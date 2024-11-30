//
// neighborhood retention mechanism
//

#include <cstdlib>
#include <ctime>

#include "const.h"
#include "tabu_search.h"
#include "ts_retain.h"

void ts_retain::add_to_retain(const struct ts_solution &S) {
    if(this->retain_set.size() >= t_retain){
        auto it = this->retain_set.end();
        it = std::next(it, -1);
        this->retain_set.erase(it);
    }
    this->retain_set.insert(S);
}

struct ts_solution ts_retain::pop_random_sol(){
    ts_solution S_rand;
    if(retain_set.size() == 0){
    	S_rand.max_time = -1;
    	return S_rand;
	}
	srand(time(NULL));
    auto it = this->retain_set.begin();
    S_rand = *it;
    if(retain_set.size() == 1){
    	return S_rand;
	}
    int k = rand() % retain_set.size();
    it = std::next(it, k);
    S_rand = *it;
    this->retain_set.erase(it);
    return S_rand;
}

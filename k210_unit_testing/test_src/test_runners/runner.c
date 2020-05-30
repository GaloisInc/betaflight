#include "./Test_Runner.h"
#include "../test_flash/test_flash.h"
#include "../test_config/test_config.h"

static void RunAllTests(void){
	RUN_TEST_GROUP(test_config);
	RUN_TEST_GROUP (test_flash);
}

int main( int argc, const char* argv[] ) {
	return UnityMain( argc, argv, RunAllTests );

}

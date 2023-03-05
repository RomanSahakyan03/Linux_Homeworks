#include <iostream>
#include "complex.hpp"


int main(){

	int n;
	float a,b;
	std::cin >> n;
	Complex *arr = new Complex[n];

   	for(int i = 0; i < n; ++i){
     		std::cin >> a >> b;
     		arr[i] = Complex(a,b);
   	}


	for(int i = 0; i < n; ++i){
    		std::cout << arr[i].abs_val() << ' ';
  	}

  	std::cout << std::endl << "Sorted:" << std::endl;

  	sorting(arr, n);

  	for(int i = 0; i < n; ++i){
    		std::cout << arr[i].abs_val() << ' ';
  	}
  

	  // Complex test1(1,2), test2(2,2);
	  // Complex res1 = test1 + test2;
	  // std::cout << res1.abs_val() << std::endl;

	  // Complex test3(8,10), test4(2,2);
	  // Complex res2 = test3 - test4;
	  // std::cout << res2.abs_val() << std::endl;

	  // Complex test5(3.5,12);
	  // Complex res3 = test5 * 2;
	  // std::cout << res3.abs_val() << std::endl;

  	delete [] arr;
  	
    return 0;
}

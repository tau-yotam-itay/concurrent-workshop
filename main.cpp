//
//  main.cpp
//  workshop
//
//  Created by Yotam Manne on 18/05/2019.
//  Copyright © 2019 Yotam Manne. All rights reserved.
//

#include <iostream>
#include "graph.cpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    Graph g(argv[1]);
    g.print_graph();
    return 0;
	
}


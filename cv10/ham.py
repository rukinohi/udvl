#!/bin/env python3
 
import os
import sys
sys.path[0:0] = [os.path.join(sys.path[0], '../examples/sat')]
import sat
 
def p(pos, i, n):
    return pos*n + i + 1;
 
class HamiltonianCycle(object):
    def find(self, edges):
        """ Finds a hamiltonian cycle in the grap givne by 'edges'
           or returns an empty list if there is none.
       """
 
        n = len(edges) # number of vertices in the graph
 
        solver = sat.SatSolver()
        w = sat.DimacsWriter('ham_in_cnf.txt')
        for pos in range(n):
            for i in range(n):
                w.writeLiteral(p(pos,i,n))
            w.finishClause()
        for pos in range(n):
            for i in range(n):
                for j in range(n):
                     if i!=j:
                        w.writeImpl(p(pos,i,n), -p(pos,j,n))
        for pos1 in range(n):
            for pos2 in range(n):
                for i in range(n):
                    if pos1!=pos2:
                        w.writeImpl(p(pos1,i,n), -p(pos2,i,n))
        for i in range(n):
            for j in range(n):
                if edges[i][j] == False:
                    for pos in range(n):
                        w.writeImpl(p(pos,i,n), -p((pos+1)%n,j,n)) 
        ok, sol = solver.solve(w, 'ham_out_cnf.txt')
        ret = [0] * n
        if ok:
            for x in sol:
                if x > 0:
                    x -= 1
                    i = x % n
                    pos = x // n
                    ret[pos] = i                   
        else:
            ret = []
        return ret
        return []
# vim: set sw=4 ts=4 sts=4 et :

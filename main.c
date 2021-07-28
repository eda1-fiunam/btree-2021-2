/*Copyright (C) 
 * 2021 - francisco.rodriguez at ingenieria dot unam dot edu
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "defs.h"
#include "data.h"
#include "Btree.h"

// Esta versión no modifica los datos originales

void print( int key )
{
   printf( "Key=%d\n", key );
}

int main()
{
   DBG_PRINT( 
         "M_ORDER=     %d\n" 
         "M_ORDER_MIN= %d\n"
         "MAX_KEYS=    %d\n"
         "MIN_KEYS=    %d\n",
         M_ORDER, M_ORDER_MIN, MAX_KEYS, MIN_KEYS );

#if 1 
   Btree* bt = Btree_New();
   // el orden del árbol, M_ORDER, está definido en defs.h

   int keys[13] = { 500,700,900,1100,600,650,1000,800,550,625,675,1200,1150};

   printf( "Filling the tree:\n" );
   for( size_t i = 0; i < 5; i++ ) {
      Btree_Insert( bt, keys[ i ] );
   }


   printf( "Traversing the tree:\n" );
   if( Btree_Traverse( bt, print ) == false ) printf( "Árbol vacío\n" ); 

   Btree_Delete( &bt );
#endif  
}

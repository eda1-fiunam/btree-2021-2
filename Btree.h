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

#ifndef  BTREE_INC
#define  BTREE_INC

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#include "defs.h"
#include "data.h"

// La documentación va en el archivo de implementación

/**
 * @brief Mantiene juntas la llave (que se usa a lo largo de todas las funciones
 * árbol) y el índice de la misma en la tabla de datos.
 */
typedef struct
{
   int key;           ///< campo de indexado. En este ejemplo Data.val
//   size_t data_index; ///< índice del elemento en la tabla de datos
} Info;


/**
 * @brief Declara lo que es un nodo
 */
typedef struct Node
{
   int keys[ (2*M_ORDER-1) ]; ///< Arreglo de llaves

   struct Node* children[ (2*M_ORDER) ]; ///< Arreglo de hijos 
   // siempre hay un hijo más que llaves
   
   size_t cnt; ///< Número de elementos actualmente en el nodo.
   // lo nombré así para que no se confunda con el número de nodos en el árbol,
   // cuyo campo se llama len.

   bool leaf;  ///< Indica si el nodo es hoja.

} Node;

typedef struct
{
   Node* root; ///< Apunta al nodo raíz del árbol.
   size_t len; ///< Número de nodos actualmente en el árbol.
} Btree;

Btree* Btree_New();
void Btree_Delete( Btree** this );
Node* Btree_Get_root( Btree* this );
void Btree_Insert( Btree* this, int key );
void Btree_Delete_all( Btree* this );
bool Btree_Traverse( Btree* this, void (*visit)(int key) );
int Btree_Find( Btree* this, int key );

#endif   /* ----- #ifndef BTREE_INC  ----- */

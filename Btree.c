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

#include "Btree.h"

// Poner la documentación en este archivo

static Node* new_node()
{
   Node* n = (Node*) malloc( sizeof( Node ) );
   if( n != NULL )
   {
      n->cnt = 0;
      n->leaf = true;

      for( size_t i = 0; i < M_ORDER; ++i ) n->keys[ i ] = EMPTY_CELL;
      // en este ejemplo no existen llaves con el valor EMPTY_CELL

      for( size_t i = 0; i < M_ORDER + 1; ++i ) n->children[ i ] = NULL;
   }

   return n;
}

Btree* Btree_New()
{
   Btree* bt = (Btree*) malloc( sizeof( Btree ) );
   if( bt != NULL )
   {
      bt->root = NULL;
      bt->len = 0;
   }

   return bt;
}

static void delete_all( Node* parent )
{
   if( !parent->leaf )
   {
      for( size_t i = 0; parent->children[ i ] != NULL; ++i )
      {
         delete_all( parent->children[ i ] );
      }
   } 
   free( parent );

   parent = NULL;
   // para depuración, NO ES NECESARIA
}

void Btree_Delete_all( Btree* this )
{
   delete_all( this->root );
}

void Btree_Delete( Btree** this )
{
   if( (*this)->root != NULL )
   {
      delete_all( (*this)->root );
      *this = NULL;
   }
}

Node* Btree_Get_root( Btree* this )
{
   return this->root;
}

static Node* split_node( Node* parent, size_t index )
{
   DBG_PRINT( "-- split_node: Splitting parent=%p at index=%ld\n", parent, index );

   Node* left = parent->children[ index ];
   // le ponemos nombre al nodo hijo a la izquierda

   // creamos el nuevo nodo:
   Node* right = new_node();
   assert( right );

   //(++this->len) // hay que pasar el árbol
   // el árbol tiene un nuevo nodo

   right->leaf = left->leaf;
   // el nuevo nodo a la derecha tiene el mismo nivel que el de la izquierda

   
   right->cnt = MIN_KEYS;
   // el nuevo nodo a la derecha debe contener el número mínimo de llaves

   // distribuímos las llaves (left -> right ):
   for( size_t j = 0; j < MIN_KEYS; ++j )
   {
//      right->keys[ j ] = left->keys[ j + MIN_KEYS + 1 ];
      right->keys[ j ] = left->keys[ j + MIN_KEYS ];

      left->keys[ j + MIN_KEYS ] = EMPTY_CELL;
      // para depuración, NO ES NECESARIA
   }


   // distribuímos los enlaces a los hijos:
   if( left->leaf == false )
   {
      for( size_t j = 0; j < M_ORDER_MIN; ++j )
      {
         right->children[ j ] = left->children[ j + M_ORDER_MIN ];

         left->children[ j + M_ORDER_MIN ] = NULL;
         // para depuración, NO ES NECESARIA
      }
   }
#if 0 
   // distribuímos los enlaces a los hijos:
   if( left->leaf == false )
   {
      for( size_t j = 0; j < MIN_KEYS; ++j )
      {
         right->children[ j ] = left->children[ j + MIN_KEYS + 1 ];

         left->children[ j + MIN_KEYS + 1 ] = NULL;
         // para depuración, NO ES NECESARIA
      }
   }
#endif  

   left->cnt = MIN_KEYS;
   // el elemento más a la derecha se va a subir al nodo padre

   // hacemos lugar en el nodo padre para el nuevo enlace:
   for( size_t j = parent->cnt; j > index; --j )
   {
      parent->children[ j + 1 ] = parent->children[ j ];

      parent->children[ j ] = NULL;
      // para depuración, NO ES NECESARIA
   }

   parent->children[ index + 1 ] = right;
   // insertamos como hijo al nuevo nodo right

   // hacemos lugar en el nodo padre para la nueva llave:
   for( size_t j = parent->cnt; j > index; --j )
   {
      parent->keys[ j ] = parent->keys[ j - 1 ];

      parent->keys[ j - 1 ] = EMPTY_CELL;
      // para depuración, NO ES NECESARIA
   }

   DBG_PRINT( "-- split_node: Inserting the median=%d into a non leaf node\n", left->keys[ MIN_KEYS ] );

   parent->keys[ index ] = left->keys[ MIN_KEYS ];
   // insertamos la mediana (es decir, el valor promocionado desde el nodo hijo izquierdo)

   left->keys[ MIN_KEYS ] = EMPTY_CELL;
   // para depuración, NO ES NECESARIA

   ++parent->cnt;
   // el nodo padre tiene un elemento más

   return parent;
}

static Node* insert_node( Node* node, int key )
{
   DBG_PRINT( "-- insert_node: Inserting key=%d into node=%p\n", key, node );

   size_t index;

   if( node->leaf == true )
   {
      DBG_PRINT( "--- insert_node: It is a leaf\n" );

      // desplaza las llaves a la derecha hasta encontrarle su lugar a key:
      for( index = node->cnt; index > 0 && key < node->keys[ index - 1 ]; --index )
      {
         node->keys[ index ] = node->keys[ index - 1 ];

         node->keys[ index - 1 ] = EMPTY_CELL;
         // para depuración, NO ES NECESARIA
      }

      node->keys[ index ] = key;
      // insertamos la llave

      ++node->cnt;
      // hay un nuevo elemento en el nodo

      //( escribe 'node' al disco)

   } else // el nodo no es hoja:
   {
      DBG_PRINT( "--- insert_node: Not a leaf\n" );

      for( index = node->cnt; index > 0 && key < node->keys[ index - 1 ]; --index ){ /* nada */ }

      //(lee del disco 'node.children[index])

      if( node->children[ index ]->cnt == MAX_KEYS )
      {
         node->leaf = false;
         // el nodo deja de ser hoja

         node = split_node( node, index );

         if( key > node->keys[ index ] ) ++index;
      }

      insert_node( node->children[ index ], key );
      // !NO LLEVA LA ASIGNACIÓN Y NO HAY QUE PONERLA!
   }

   return node;
}



void Btree_Insert( Btree* this, int key )
{
   DBG_PRINT( "Inserting key=%d\n", key );

   if( this->root == NULL )
   {
      this->root = new_node();
      assert( this->root );

      this->root->keys[ 0 ] = key;
      this->root->cnt = 1;
   } else
   {
      if( this->root->cnt == MAX_KEYS )
      {
         Node* old_root = this->root;

         this->root = new_node();
         assert( this->root );

         //++this->len;

         this->root->children[ 0 ] = old_root;
         this->root->leaf = false;
         this->root = split_node( this->root, 0 );
      }
      this->root = insert_node( this->root, key );
   }

   assert( this->root != NULL );
   // ¿qué dispara la aserción?
}

static void traverse_node( Node* node, void (*visit)(int key) )
{
   if( node->leaf == true )
   {
      for( size_t i = 0; i < node->cnt; ++i )
      {
         visit( node->keys[ i ] );
      }
   } else
   {
      for( size_t i = 0; i < node->cnt; ++i )
      {
         traverse_node( node->children[ i ], visit );
         // desciende en el árbol hasta encontrar una hoja

         if( i < node->cnt )
         {
            visit( node->keys[ i ] );
         }
      }
   }
}

bool Btree_Traverse( Btree* this, void (*visit)(int key) )
{
   if( this->root == NULL ) return false;

   traverse_node( this->root, visit );
   return true;
}

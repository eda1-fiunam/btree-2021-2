/*Copyright (C) 
 * 
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
 * 2021 - francisco dot rodriguez at ingenieria dot unam dot mx
 */



#ifndef  DEFS_INC
#define  DEFS_INC

//#define DBG_HELP 0
// definir en la línea de comandos, -DDBG_HELP=x, x = 0 o 1

#ifdef DBG_HELP
#include <stdio.h>
#endif  

#if DBG_HELP == 1
#define DBG_PRINT( ... ) \
   do{\
      fprintf( stderr, "DBG:" __VA_ARGS__ ); \
   } while( 0 )
#else
#define DBG_PRINT( ... ) ;
#endif  

#define M_ORDER 5 ///< Orden del árbol (máximo número de hijos por nodo)

#define M_ORDER_MIN ( (int)( (M_ORDER) / 2 + ( (M_ORDER) % 2 !=  0 ) ) ) ///< M_ORDER_MIN = ceil( M_ORDER / 2 )

#define MAX_KEYS ( M_ORDER - 1 ) ///< Número máximo de LLAVES en el nodo

//#define MIN_KEYS ( M_ORDER_MIN - 1 ) ///< Número mínimo de LLAVES en el nodo
#define MIN_KEYS ( MAX_KEYS / 2 ) ///< Número mínimo de LLAVES en el nodo
//#define MAX_CHILDREN (MAX_KEYS)

#define EMPTY_CELL -1

#endif   /* ----- #ifndef DEFS_INC  ----- */

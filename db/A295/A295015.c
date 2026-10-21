/*
 * A295015  Squares whose largest digit is 5.
 * https://oeis.org/A295015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 21, u32 n <= 1026, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits_largest_is_five
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Squares whose largest decimal digit is exactly 5.
   We generate natural numbers m in increasing order, square them,
   and keep those squares whose decimal representation contains
   at least one digit 5 and no digit >5.  The sequence a(n) is the
   nth such square,  a(1)=25, a(2)=225, …
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/digits_largest_is_five.h"

/* Cache storing the squares that satisfy the condition. */
static seq_t *cache      = NULL;   /* array of squares found so far */
static size_t cache_cap  = 0;      /* allocated capacity of cache */
static size_t cache_len  = 0;      /* number of squares stored */
static idx_t  last_checked = 0;    /* last integer that was examined */

/*---------------------------------------------------------------*/
/*   seq_t a(idx_t n)     */
/*---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Expand the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Next integer to test */
        ++last_checked;
        seq_t m = (seq_t)last_checked;

        /* Square it */
        seq_t sq = m * m;

        /* Check whether the square satisfies the digit condition. */
        if (digits_largest_is_five(sq)) {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failed – terminate the program. */
                    exit(EXIT_FAILURE);
                }
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = sq;
        }
    }
    return cache[n - 1];
}

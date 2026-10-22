/*
 * A322045  Triangular numbers that are 2-digit-repblocks.
 * https://oeis.org/A322045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implements OEIS A322045.  a(n) is the n‑th triangular number
    that is a 2‑digit repblock. */
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Local cache of the sequence.  The cache is built incrementally on
   the first call and extended on demand. */
static seq_t *cache = NULL;          /* array of values */
static size_t cache_sz = 0;          /* number of cached values */
static size_t cache_cap = 0;         /* current capacity of the array */

/* State for generating the next repblock candidate.  We generate repblocks
   in increasing order by number of repetitions (r) and by the 2‑digit block
   value (b).  The variables are static so that subsequent calls resume
   where the previous one left off. */
static idx_t next_r = 1;     /* current number of repetitions */
static unsigned char next_block = 10;  /* current 2‑digit block */

/*  a(idx_t n) ------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Sanity: the sequence is defined for n >= 1.  If n < 1 we return 0. */
    if (n < 1)
        return (seq_t)0;

    /* Extend the cache until it contains at least n terms. */
    while (cache_sz < (size_t)n) {

        /* Build the candidate repblock value */
        seq_t rep = (seq_t)0;
        seq_t block = (seq_t)next_block;   /* the 2‑digit block 10..99 */
        for (idx_t i = 0; i < next_r; ++i)
            rep = rep * (seq_t)100 + block;   /* rep = rep*100 + block */

        /* Test whether rep is a triangular number:
           1 + 8*rep must be a perfect square. */
        seq_t check = rep * (seq_t)8 + (seq_t)1;
        seq_t root  = floor_sqrt(check);
        if (root * root == check) {
            /* Store the value in the cache.  Resize if necessary. */
            if (cache_sz == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_arr) /* memory exhausted – abort the program */
                    abort();
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = rep;
        }

        /* Advance to the next repblock candidate. */
        ++next_block;
        if (next_block > 99) {
            next_block = 10;
            ++next_r;
        }
    }

    /* Return the requested term.  cache indexing starts at 0. */
    return cache[n - 1];
}

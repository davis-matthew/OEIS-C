/*
 * A382631  Integers whose binary representation contains exactly four 1's, no
 *          two 1's being adjacent.
 * https://oeis.org/A382631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based enumeration of integers whose binary representation
 * contains exactly four 1's and no two of them are adjacent.
 */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *lookup = NULL;      /* cache of generated terms      */
static idx_t   sz    = 0;         /* number of cached terms        */
static idx_t   cap   = 0;         /* allocated size of lookup[]    */
static seq_t   probe = 0;         /* next integer to test         */

/* Return true if x has exactly 4 ones and no two adjacent ones. */
static bool is_valid(seq_t x)
{
    if (x == 0) return false;

    /* no two adjacent ones */
    if ((x & (x >> 1)) != 0) return false;

    /* count ones; abort early if more than 4 */
    int  cnt = 0;
    seq_t y = x;
    while (y)
    {
        y &= y - 1;
        ++cnt;
        if (cnt > 4) return false;
    }
    return cnt == 4;
}

/* Grow the cache so that it contains at least n terms. */
static void ensure_cache(idx_t n)
{
    if (!lookup)        /* first call – allocate initial storage */
    {
        cap = 64;
        lookup = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!lookup) abort();
        sz    = 0;
        probe = 0;
    }

    while (sz < n)     /* generate more terms if needed */
    {
        seq_t x = probe + 1;
        for (;; ++x)          /* search for next valid integer */
        {
            if (is_valid(x))
            {
                if (sz == cap)   /* grow array if full */
                {
                    cap *= 2;
                    lookup = (seq_t *)realloc(lookup, cap * sizeof(seq_t));
                    if (!lookup) abort();
                }
                lookup[sz++] = x;
                probe        = x;
                break;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1 – behaviour below is irrelevant */
    ensure_cache(n);
    return lookup[n - 1];
}

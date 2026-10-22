/*
 * A396060  Numbers m such that m^2 + 1 and m^2 - m + 1 are both prime
 *          numbers.
 * https://oeis.org/A396060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 529, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq_array, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n-th integer m (n ≥ 1) such that both m^2+1 and
 * m^2−m+1 are prime.  The function generates terms on demand and
 * memoises them.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/grow_seq_array.h"

static seq_t *cache = NULL;   /* memoised sequence values */
static size_t cap   = 0;      /* current allocated capacity of cache */
static size_t sz    = 0;      /* number of stored terms */
static seq_t last_m = 0;      /* last m tested */

/* Return the n-th term of A396060 (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0)    /* offset starts at 1; 0 is not a valid index */
        return 0;

    /* Generate terms until we have at least n terms stored */
    while (sz < n)
    {
        /* Next candidate for m */
        seq_t m = last_m + 1;
        last_m = m;

        seq_t sq = m * m;
        seq_t p1 = sq + 1;        /* m^2 + 1   */
        seq_t p2 = sq - m + 1;    /* m^2 - m + 1 */

        if (isPrimeGeneric(p1) && isPrimeGeneric(p2))
        {
            /* Ensure sufficient capacity and store the value */
            grow_seq_array(&cache, &cap, sz + 1);
            cache[sz++] = m;
        }
    }

    return cache[n - 1];
}

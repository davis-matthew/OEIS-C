/*
 * A274515  a(n) is the number of times that the value of ternary n when read
 *          as hyperbinary occurs in the set of hyperbinary representations.
 * https://oeis.org/A274515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A002487(A065361(n)+1) where A065361(n) interprets the
 *            ternary digits of n as a hyperbinary representation and A002487
 *            is the Stern diatomic sequence
 * Range:     u8 n <= 3746, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include only what the build system supplies. */
#include <stdlib.h>

/* Forward declaration of the Stern function. */
static seq_t stern(idx_t k);

/* Compute a(n). */
seq_t a(idx_t n)
{
    /* 1. Map n into a hyperbinary value m:
       m = sum d_i * 2^i where d_i are the base‑3 digits of n. */
    seq_t m = 0;
    seq_t power = 1;          /* 2^i */
    idx_t tmp = n;

    while (tmp > 0) {
        int d = tmp % 3;      /* base‑3 digit (0,1,2) */
        m += (seq_t)d * power;
        tmp /= 3;
        power <<= 1;          /* multiply by 2 for the next bit position */
    }

    /* 2. a(n) = Stern(m+1) */
    idx_t idx = (idx_t)(m + 1);
    return stern(idx);
}

/* ----------------------------------------------
 *   Stern's diatomic sequence (A002487)
 * ---------------------------------------------- */

/* Cache for already computed values. */
static seq_t *stern_cache = NULL;
static size_t stern_cap   = 0;

/* Ensure the cache has room for indices up to `max_n`. */
static void grow_cache_to(idx_t max_n)
{
    /* The cache must be at least max_n+1 long. */
    if (stern_cap > (size_t)max_n)
        return;

    /* Determine new capacity, doubling until large enough. */
    size_t new_cap = stern_cap ? stern_cap * 2 : 8;
    while (new_cap <= (size_t)max_n)
        new_cap <<= 1;

    /* Reallocate. */
    stern_cache = (seq_t *)realloc(stern_cache, new_cap * sizeof(seq_t));

    /* Initialise the newly added slots. */
    if (stern_cap == 0) {
        stern_cache[0] = 0;
        stern_cache[1] = 1;
        stern_cap = 2;
    }

    /* Compute values for indices from stern_cap up to new_cap-1. */
    for (idx_t i = stern_cap; i < (idx_t)new_cap; ++i) {
        if ((i & 1) == 0) {              /* even index */
            stern_cache[i] = stern_cache[i >> 1];
        } else {                         /* odd index */
            stern_cache[i] = stern_cache[i >> 1]
                            + stern_cache[(i >> 1) + 1];
        }
    }

    stern_cap = new_cap;
}

/* Stern value with cache. */
static seq_t stern(idx_t k)
{
    /* Base cases are automatically stored in the cache. */
    grow_cache_to(k);
    return stern_cache[k];
}

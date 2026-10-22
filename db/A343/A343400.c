/*
 * A343400  Number of ways to write n as 3^x + [y^2/3] + [z^2/4], where [.] is
 *          the floor function, x is a nonnegative integer, and y and z are
 *          positive integers.
 * https://oeis.org/A343400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = number of ways to write n as 3^x + floor(y^2/3) +
 *            floor(z^2/4) where x is a nonnegative integer and y,z are
 *            positive integers
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Static tables for memoization */
static seq_t *pairCount = NULL;        /* counts of sum of floor(y^2/3)+floor(z^2/4) */
static size_t pairCap = 0;             /* allocated size of pairCount (>= max n+1) */
static seq_t pow3_cache[64];           /* powers of 3, size <= 64 is enough for all n <= 2^64-1 */
static size_t pow3_size = 0;           /* number of powers of 3 stored */
static idx_t currentMaxN = 0;          /* largest n already processed */

/* Recompute tables up to new maximum n */
static void recompute_tables(idx_t n)
{
    /* free old table if any */
    if (pairCount != NULL) {
        free(pairCount);
    }

    /* allocate new pairCount array of length n+1 */
    pairCap = (size_t)n + 1;
    pairCount = (seq_t *)calloc(pairCap, sizeof(seq_t));
    if (pairCount == NULL) { /* allocation failed – abort */
        exit(EXIT_FAILURE);
    }

    /* compute floor(y^2/3) values up to n */
    for (idx_t y = 1; ; ++y) {
        seq_t ay = ((seq_t)y * (seq_t)y) / 3;
        if (ay > n) break;

        /* compute floor(z^2/4) values where ay + az <= n */
        for (idx_t z = 1; ; ++z) {
            seq_t az = ((seq_t)z * (seq_t)z) / 4;
            seq_t sum = ay + az;
            if (sum > n) break;
            pairCount[(size_t)sum]++;   /* increment count of this pair sum */
        }
    }

    /* recompute powers of 3 table */
    pow3_size = 0;
    seq_t p = 1;
    while (p <= n) {
        pow3_cache[pow3_size++] = p;
        p *= 3;
    }

    /* update current maximum processed */
    currentMaxN = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0, return 0 to avoid UB */

    /* Ensure tables are computed up to n */
    if (n > currentMaxN) {
        recompute_tables(n);
    }

    seq_t count = 0;
    /* sum over all possible 3^x <= n */
    for (size_t i = 0; i < pow3_size; ++i) {
        seq_t rem = n - pow3_cache[i];
        count += pairCount[(size_t)rem];
    }

    return count;
}

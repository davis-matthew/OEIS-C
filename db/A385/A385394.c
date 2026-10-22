/*
 * A385394  a(n) = (Sum_{k=0..n} (binomial(n, k) mod 8)) / 2^bitcount(n).
 * https://oeis.org/A385394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Sum_{k=0..n} (binomial(n, k) mod 8)) / 2^bitcount(n)
 * Range:     u8 n <= 62, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* static cache for previously computed values */
static idx_t          maxComputed = 0;   /* highest n already computed   */
static seq_t       *    aCache   = NULL; /* a(n) values                  */
static uint8_t     *    binRow   = NULL; /* current row of C(n,k) mod 8  */
static size_t          binCap   = 0;     /* allocated capacity for binRow */

/* popcount for an unsigned 64‑bit integer */
static unsigned int popcnt(idx_t x)
{
    unsigned int c = 0;
    while (x)
    {
        x &= x - 1;
        ++c;
    }
    return c;
}

/* compute a(n) as defined above, caching results up to the maximum n seen */
seq_t a(idx_t n)
{
    /* initial allocation when needed */
    if (maxComputed == 0 && aCache == NULL)
    {
        size_t initCap = (size_t) (n + 1);
        if (initCap == 0) initCap = 1;                 /* at least one slot */
        binRow = (uint8_t*)malloc(initCap * sizeof(uint8_t));
        aCache = (seq_t*)malloc(initCap * sizeof(seq_t));
        binCap = initCap;

        /* row 0 */
        binRow[0] = 1;
        aCache[0] = 1;
        maxComputed = 0;
    }

    /* extend precomputed values up to n */
    if (n > maxComputed)
    {
        size_t newCap = (size_t)(n + 1);
        if (newCap > binCap)
        {
            binRow = (uint8_t*)realloc(binRow, newCap * sizeof(uint8_t));
            aCache = (seq_t*)realloc(aCache, newCap * sizeof(seq_t));
            binCap = newCap;
        }

        for (idx_t i = maxComputed + 1; i <= n; ++i)
        {
            /* prepare the new entry */
            binRow[i] = 0;

            /* update binRow in place using Pascal's rule */
            for (idx_t k = i; k >= 1; --k)
            {
                uint8_t val = (binRow[k    ] + binRow[k - 1]) & 0x7; /* mod 8 */
                binRow[k] = val;
            }

            /* sum the row (as full integers, not mod 8) */
            seq_t sum = 0;
            for (idx_t k = 0; k <= i; ++k)
                sum += (seq_t)binRow[k];

            /* divide by 2^bitcount(i) */
            unsigned int bc = popcnt(i);
            seq_t denom = ((seq_t)1 << bc);
            aCache[i] = sum / denom;
        }
        maxComputed = n;
    }

    return aCache[n];
}

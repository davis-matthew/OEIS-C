/*
 * A332245  Positive integers represented by the ternary quadratic form 5x^2 +
 *          8y^2 + 8z^2 - 5yz - xz - 4xy; equally, positive integers
 *          represented by the ternary quadratic form 5x^2 + 5y^2 + 8z^2 - yz
 *          - 4xz - 2xy.
 * https://oeis.org/A332245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A332245:  Representable integers of the quadratic form  
   5x² + 8y² + 8z² – 5yz – xz – 4xy.  
   Computed by exhaustive search up to a fixed upper bound. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

/* Integer square root for unsigned 64‑bit numbers. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    idx_t bit = 1ull << 62;        /* highest power of 4 ≤ 2^64 */
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Global data for memoization */
static seq_t *values = NULL;      /* sorted list of all representable numbers */
static size_t  values_len = 0;     /* number of stored terms   */
static int     initialized = 0;   /* flag whether initialized */

/* Pre‑compute all representable numbers up to LIMIT. */
static void init_sequence(void)
{
    if (initialized) return;
    /* Decide on a fixed limit that covers many thousands of terms. */
    const idx_t LIMIT = 200000ull;  /* safe upper bound for n≈10000 */

    /* Allocate a bitmap for numbers up to LIMIT */
    unsigned char *seen = (unsigned char *)calloc((size_t)(LIMIT + 1), sizeof(unsigned char));
    assert(seen != NULL);

    /* Bounds for variables */
    const idx_t xmax = isqrt_u64(LIMIT / 5) + 1;   /* 5x^2 ≤ LIMIT */
    const idx_t ymax = isqrt_u64(LIMIT / 8) + 1;   /* 8y^2 ≤ LIMIT */
    const idx_t zmax = isqrt_u64(LIMIT / 8) + 1;   /* 8z^2 ≤ LIMIT */

    /* Enumerate all triples (x,y,z) with |x|≤xmax, |y|≤ymax, |z|≤zmax */
    for (idx_t ix = 0; ix <= 2 * xmax; ++ix) {
        long long x = (long long)ix - (long long)xmax;
        for (idx_t iy = 0; iy <= 2 * ymax; ++iy) {
            long long y = (long long)iy - (long long)ymax;
            for (idx_t iz = 0; iz <= 2 * zmax; ++iz) {
                long long z = (long long)iz - (long long)zmax;

                long long m = 5LL * x * x
                              + 8LL * y * y
                              + 8LL * z * z
                              - 5LL * y * z
                              - x * z
                              - 4LL * x * y;

                if (m >= 1 && (idx_t)m <= LIMIT)
                    seen[(size_t)m] = 1U;
            }
        }
    }

    /* Count and store the representable numbers in increasing order */
    values_len = 0;
    for (idx_t v = 1; v <= LIMIT; ++v)
        if (seen[(size_t)v]) ++values_len;

    values = (seq_t *)malloc(values_len * sizeof(seq_t));
    assert(values != NULL);

    size_t pos = 0;
    for (idx_t v = 1; v <= LIMIT; ++v)
        if (seen[(size_t)v]) values[pos++] = (seq_t)v;

    free(seen);
    initialized = 1;
}

/* Return the nth term (offset 1) of A332245. */
seq_t a(idx_t n)
{
    assert(n >= 1);               /* sequence is 1‑based */
    if (!initialized) init_sequence();
    assert(n <= values_len);      /* for requested n we have enough terms */
    return values[(size_t)(n - 1)];
}

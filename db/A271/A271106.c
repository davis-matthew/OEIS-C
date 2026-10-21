/*
 * A271106  Number of ordered ways to write n as x^6 + 3*y^3 + z^3 +
 *          w*(w+1)/2, where x and y are nonnegative integers, and z and w are
 *          positive integers.
 * https://oeis.org/A271106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‐force search for representations of n as
   x^6 + 3*y^3 + z^3 + w*(w+1)/2
   (x,y ≥ 0, z,w ≥ 1).  Counts the number of ordered
   4‑tuples (x,y,z,w) satisfying the equation. */
#include <stdlib.h>

/* Integer floor square root for unsigned 64‑bit values. */
static inline idx_t int_sqrt(idx_t n) {
    idx_t low = 0, high = (idx_t)1 << 32;          /* 2^32 > sqrt(2^64) */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) { low = 1; continue; }
        if (mid <= n / mid)        /* mid*mid <= n */
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;                     /* floor(sqrt(n)) */
}

/* Integer floor cube root for unsigned 64‑bit values. */
static inline idx_t int_cube_root(idx_t n) {
    idx_t low = 0, high = (idx_t)1 << 21;          /* 2^21^3 > 2^63 */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) { low = 1; continue; }
        /* mid^3 <= n  ⇔  mid <= n/(mid^2)  ⇔  mid <= (n/mid)/mid */
        if (mid <= (n / mid) / mid)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;                     /* floor(cuberoot(n)) */
}

seq_t a(idx_t n)
{
    /* Allocate an array that will hold, for each t from 0 to n,
       the number of positive (z,w) pairs such that
       t = z^3 + w*(w+1)/2.  The array is zero‑initialized. */
    seq_t *t_counts = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!t_counts) return 0;                 /* allocation failed */

    /* Precompute all (z,w) pairs that contribute to any t ≤ n. */
    idx_t zmax = int_cube_root(n);
    for (idx_t z = 1; z <= zmax; ++z) {
        idx_t z3 = z * z * z;                /* z^3, fits in idx_t for n ≤ 1e18 */
        for (idx_t w = 1; ; ++w) {
            idx_t tri = w * (w + 1) / 2;     /* w*(w+1)/2, fits in idx_t */
            idx_t t_val = z3 + tri;
            if (t_val > n) break;
            ++t_counts[(size_t)t_val];
        }
    }

    seq_t count = 0;

    /* Enumerate x and y, using the precomputed t_counts for the remaining
       part of the equation. */
    for (idx_t x = 0;; ++x) {
        idx_t x6 = x * x * x * x * x * x;   /* x^6, fits in idx_t for n ≤ 1e18 */
        if (x6 > n) break;
        idx_t rem1 = n - x6;                /* left after subtracting x^6 */

        for (idx_t y = 0;; ++y) {
            idx_t y3 = y * y * y;           /* y^3 */
            idx_t three_y3 = y3 + y3 + y3;  /* 3*y^3 */
            if (three_y3 > rem1) break;
            idx_t rem2 = rem1 - three_y3;   /* must equal z^3 + w*(w+1)/2 */

            count += t_counts[(size_t)rem2];
        }
    }

    free(t_counts);
    return count;
}

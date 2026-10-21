/*
 * A275648  Number of ordered ways to write n as x^2*(1+y^2+z^2)+w^2, where x
 *          is a positive integer and y,z,w are nonnegative integers with y <=
 *          z <= w.
 * https://oeis.org/A275648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A275648:  
   a(n) = #{(x,y,z,w) ∈ ℕ⁴ | x>0, 0≤y≤z≤w, n = x²·(1 + y² + z²) + w²}.  
   Computed by enumerating all admissible quadruples up to n. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *counts = NULL;   /* counts[0] unused, counts[n] holds a(n) */
static idx_t current_limit = 0;/* highest n that has been computed */

/* Integer square root for idx_t (uses binary search, safe for 64‑bit) */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n)   low = mid + 1;
        else          high = mid - 1;
    }
    return high;
}

/* Recompute counts array up to (and including) max_n */
static void recompute_counts(idx_t max_n) {
    /* Allocate new array, zero‑initialized */
#if defined(__cplusplus)
    seq_t *new_counts = new seq_t[max_n + 1];
#else
    seq_t *new_counts = calloc((size_t)(max_n + 1), sizeof(seq_t));
#endif

    /* Enumerate all admissible quadruples */
    idx_t x_max = isqrt_u64(max_n);
    for (idx_t x = 1; x <= x_max; ++x) {
        seq_t xsq = (seq_t)x * (seq_t)x;   /* x² */
        for (idx_t y = 0;; ++y) {
            seq_t ysq = (seq_t)y * (seq_t)y;                 /* y² */
            seq_t xy_const = xsq * ((seq_t)1 + ysq);         /* x²·(1 + y²) */
            if (xy_const > (seq_t)max_n) break;              /* no further y are useful */

            for (idx_t z = y; ; ++z) {
                seq_t zsq = (seq_t)z * (seq_t)z;                 /* z² */
                seq_t base = xy_const + xsq * zsq;               /* x²·(1 + y² + z²) */
                if (base > (seq_t)max_n) break;                 /* no further z are useful */

                seq_t diff = (seq_t)max_n - base;                /* w² ≤ diff */
                idx_t wmax = isqrt_u64((idx_t)diff);
                for (idx_t w = z; w <= wmax; ++w) {
                    seq_t tn = base + (seq_t)w * (seq_t)w;       /* n = base + w² */
                    idx_t idx_n = (idx_t)tn;                    /* 0 ≤ idx_n ≤ max_n */
                    new_counts[idx_n] += (seq_t)1;              /* increment count */
                }
            }
        }
    }

    /* Free old array and install the new one */
#if defined(__cplusplus)
    delete[] counts;
#else
    free(counts);
#endif
    counts = new_counts;
    current_limit = max_n;
}

/* Public sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset is 1; undef for n==0 */
    if (n > current_limit) {
        recompute_counts(n);
    }
    return counts[(size_t)n];
}

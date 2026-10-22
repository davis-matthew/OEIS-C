/*
 * A347784  Number of compositions (ordered partitions) of n into at most 3
 *          powers of 2.
 * https://oeis.org/A347784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A347784: number of compositions of n into at most 3 powers of 2.
   The function iterates over the powers of 2 up to n and counts
   all ordered tuples of length 1, 2, or 3 whose sum is n. */
 
#include <stddef.h>       /* for size_t */

seq_t a(idx_t n)
{
    /* Handle the special case n = 0 (empty composition). */
    if (n == 0)
        return 1;

    seq_t count = 0;

    /* Precompute all powers of two ≤ n. */
    enum { MAX_POW = 64 };             /* > log2(UINT64_MAX) */
    idx_t pow2[MAX_POW];
    size_t np = 0;
    idx_t v = 1;
    while (v <= n) {
        pow2[np++] = v;
        if (v > n / 2)
            break;
        v <<= 1;
    }

    /* 1-part compositions: n itself must be a power of two. */
    if ((n & (n - 1)) == 0)   /* n > 0 because n==0 handled above */
        count += 1;

    /* 2-part compositions: count each unordered pair once
       and multiply by 2 if the parts differ. */
    for (size_t i = 0; i < np; ++i) {
        idx_t p = pow2[i];
        if (p >= n)                /* p ≤ n-1 */
            break;
        idx_t q = n - p;
        if (q > 0 && (q & (q - 1)) == 0) {
            if (p < q)
                count += 2;
            else if (p == q)
                count += 1;
        }
    }

    /* 3-part compositions: iterate over all ordered pairs (p,q)
       and check if the remaining part r = n-p-q is a power of two. */
    if (n >= 3) {
        for (size_t i = 0; i < np; ++i) {
            idx_t p = pow2[i];
            if (p > n - 2)
                break;          /* need at least two more units for q,r */
            for (size_t j = 0; j < np; ++j) {
                idx_t q = pow2[j];
                if (p + q > n - 1)
                    break;      /* need at least one unit for r */
                idx_t r = n - p - q;
                if (r > 0 && (r & (r - 1)) == 0)
                    count += 1;
            }
        }
    }

    return count;
}

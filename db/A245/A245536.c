/*
 * A245536  Write n>=1 as either n=2^k-2^r with 0 <= r <= k-1, in which case
 *          a(2^k-2^r)=k-r-1, or as n=2^k-2^r+j with 2 <= r <= k-1, 1 <= j <
 *          2^r-1, in which case a(2^k-2^r+j)=(k-r-1)*a(j).
 * https://oeis.org/A245536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A245536: 
 *   a(2^k-2^r)     = k - r - 1                     (0 ≤ r ≤ k-1)
 *   a(2^k-2^r + j) = (k - r - 1) * a(j)           (2 ≤ r ≤ k-1, 1 ≤ j < 2^r-1)
 *
 * Offset: 1  (a(1) is the first term)
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* safety padding  (OEIS offset requires n≥1) */

    /* Determine k so that 2^k > n ≥ 2^(k-1)  */
    int k = 64 - __builtin_clzll(n);           /* k ∈ [1, 64] */

    /* 2^k as an unsigned 128‑bit value to avoid overflow for k == 64 */
    __uint128_t powk = ((__uint128_t)1) << k;   /* 2^k  */

    uint64_t base = 0;                        /* 2^k - 2^r  */
    int r = 0;                                

    /* Find the smallest r (largest base) satisfying base ≤ n  */
    for (int r_candidate = 0; r_candidate < k; ++r_candidate) {
        uint64_t powr = (uint64_t)1 << r_candidate;          /* 2^r_candidate  */
        __uint128_t base128 = powk - ((__uint128_t)powr);    /* 2^k - 2^r  */
        if ((uint64_t)base128 <= n) {                      /* base fits in 64 bits */
            r = r_candidate;
            base = (uint64_t)base128;
            break;
        }
    }

    /* Base case: n is of the form 2^k - 2^r  */
    if (n == base) {
        return (seq_t)(k - r - 1);
    }

    /* General case: n = 2^k - 2^r + j  with j > 0  */
    idx_t j = n - base;                           /* 1 ≤ j < 2^r */
    seq_t aj = a(j);                              /* recursive call */
    seq_t factor = (seq_t)(k - r - 1);
    return factor * aj;
}

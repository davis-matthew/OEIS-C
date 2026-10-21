/*
 * A288871  Triangle t needed for the e.g.f.s of the column sequences of
 *          A288870 with leading zeros.
 * https://oeis.org/A288871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A288871
 *  Formula:  a(n) = t(k,m)  where the pair (k,m) is obtained by
 *  row‑major enumeration of the triangle  
 *    t(k,m) = 2^k + k·2^m + 2^(m-1)   (k ≥ m ≥ 1),
 *  i.e. k is the unique integer such that
 *        k(k-1)/2 <  n  ≤  k(k+1)/2,
 *  and   m = n – k(k-1)/2.
 */

#include <stdint.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

/* Compute a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    /* Find k such that k(k+1)/2 ≥ n and (k-1)k/2 < n  */
    idx_t low = 1;
    idx_t high = 1;
    /* Expand high until it satisfies the inequality */
    while ((unsigned __int128)high * (high + 1) / 2 < (unsigned __int128)n) {
        high <<= 1;
    }
    /* Binary search for the smallest k with k(k+1)/2 ≥ n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 s = (unsigned __int128)mid * (mid + 1) / 2;
        if (s >= (unsigned __int128)n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t k = low;

    /* Compute m = n - (k-1)k/2 */
    unsigned __int128 prev_sum = (unsigned __int128)(k - 1) * k / 2;
    idx_t m = (idx_t)((unsigned __int128)n - prev_sum);   /* 1‑based */

    /* Compute the term t(k,m) = 2^k + k*2^m + 2^(m-1) */
    seq_t pow2_k  = pow2(k);        /* 2^k   */
    seq_t pow2_m  = pow2(m);        /* 2^m   */
    seq_t pow2_m1 = (m > 0) ? pow2(m - 1) : 0;   /* 2^(m-1), m>=1 */

    seq_t km = (seq_t)k * pow2_m;   /* k * 2^m */
    return pow2_k + km + pow2_m1;
}

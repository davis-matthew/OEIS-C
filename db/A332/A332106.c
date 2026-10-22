/*
 * A332106  Least m > n such that n + ... + m = p*2^k with p prime and k >= 0.
 * https://oeis.org/A332106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = min{ p - n, q + n - 1 }
 *   where p = 2^r + 2n - 1  (smallest prime of this form)
 *         q = 2^s - 2n + 1  (smallest prime of this form)
 *
 * The returned value is the least m > n such that
 * n + (n+1) + … + m = p * 2^k with p prime and k ≥ 0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* handle the special case n = 0 explicitly */
    if (n == 0U)
        return (seq_t)2U;

    /* two_n_minus1 = 2*n - 1, used in both formulas */
    seq_t two_n_minus1 = (seq_t)2U * (seq_t)n - (seq_t)1U;

    /* ---- find smallest prime p of the form 2^r + 2n - 1, r ≥ 1 ---- */
    seq_t pow2 = (seq_t)1U;          /* start with 2^0 = 1, then shift to r = 1 */
    seq_t candidate = 0;
    for (;;) {
        pow2 <<= 1U;                /* move to the next power of two (r ≥ 1) */
        candidate = pow2 + two_n_minus1;
        if (isPrimeGeneric(candidate))
            break;                  /* found the required prime p */
    }
    seq_t m1 = candidate - n;      /* candidate (= p) > 2n, so m1 > n */

    /* ---- find smallest prime q of the form 2^s - 2n + 1, pow2 > 2n ---- */
    pow2 = (seq_t)1U;
    /* make pow2 the smallest power of two strictly exceeding 2*n */
    while (pow2 <= (seq_t)2U * (seq_t)n)
        pow2 <<= 1U;

    candidate = 0;
    for (;;) {
        candidate = pow2 - two_n_minus1;   /* q = 2^s - 2n + 1 */
        if (candidate > (seq_t)1U && isPrimeGeneric(candidate))
            break;
        pow2 <<= 1U;                /* try next power of two */
    }
    seq_t m2 = candidate + n - (seq_t)1U;   /* q > 1 guarantees m2 > n */

    /* ---- choose the smaller m ---- */
    return (m1 < m2) ? m1 : m2;
}

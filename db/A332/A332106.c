/*
 * A332106  Least m > n such that n + ... + m = p*2^k with p prime and k >= 0.
 * https://oeis.org/A332106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 70 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
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
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

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

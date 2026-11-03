/*
 * A350703  a(n) is the least integer k such that (2*n*k+1) | (2^k-1).
 * https://oeis.org/A350703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that (2*n*k+1) divides (2^k-1)
 * Range:     u8 n <= 26, u16 n <= 1237, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9500 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation performs a straightforward search over k.  It uses
   128‑bit integer arithmetic to avoid overflow when the modulus
   2*n*k+1 exceeds the width of seq_t.  The algorithm is correct for all
   indices that fit within the computational limits stated in the
   requirements. */

#include <stdint.h>
#include <stdbool.h>

/* Test whether 2^k ≡ 1 (mod 2*n*k+1).  The modulus is computed
   in unsigned __uint128_t to preserve correctness for values that
   might exceed seq_t.  The exponentiation uses the standard
   binary‑exponentiation method.  */
static inline bool modpow_leads_to_one(idx_t n, idx_t k)
{
    __uint128_t m = (__uint128_t)2 * n * k + 1;   /* modulus */

    __uint128_t result = 1;
    __uint128_t base   = 2 % m;
    idx_t exp          = k;

    while (exp) {
        if (exp & 1)
            result = (result * base) % m;
        base   = (base * base) % m;
        exp >>= 1;
    }
    return result == 1;
}

seq_t a(idx_t n)
{
    for (idx_t k = 1; ; ++k) {
        if (modpow_leads_to_one(n, k))
            return (seq_t)k;
    }
}

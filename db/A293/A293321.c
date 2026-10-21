/*
 * A293321  The integer k that minimizes |k/2^n - phi^2|, where phi =
 *          (1+sqrt(5))/2 = golden ratio.
 * https://oeis.org/A293321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(1/2 + r*2^n) , r = (3+√5)/2 Equivalently a(n) =
 *            (3*2^n + floor(√5*2^n) + 1) / 2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 112,
 *            big any n
 * Helpers:   floor_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include "../utils/pow2.h"       /* pow2(exp) -> seq_t 2^exp   */
#include "../utils/floor_sqrt.h" /* floor_sqrt(x) -> floor(√x) */

/* Integer square root of a 128‑bit unsigned value.
   Returns ⌊√x⌋.  This helper is used only for the 64‑bit build. */
static uint64_t sqrt128(unsigned __int128 x)
{
    uint64_t low = 0, high = UINT64_MAX, ans = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* If seq_t is larger than 64 bits we may safely use the generic
       expression involving pow2().  For small types we use an
       intermediate 128‑bit computation that works for all n whose
       result fits. */
    if (sizeof(seq_t) > sizeof(uint64_t)) {
        seq_t pow2_n  = pow2(n);
        seq_t pow2_2n = pow2(2 * n);
        seq_t d = 5 * pow2_2n;          /* D = 5 * 4^n */
        seq_t s = floor_sqrt(d);        /* s = ⌊√(5*4^n)⌋ = ⌊√5·2^n⌋ */
        return (3 * pow2_n + s + 1) / 2;
    }

    /* 64‑bit (or narrower) build */
    if (n >= 63)          /* 2^n would overflow 64 bits; result does not fit */
        return 0;

    /* 128‑bit intermediates for 64‑bit seq_t */
    unsigned __int128 pow2n   = (unsigned __int128)1 << n;   /* 2^n   */
    unsigned __int128 pow2_2n = (unsigned __int128)1 << (2 * n); /* 2^(2n) */
    unsigned __int128 d       = (unsigned __int128)5 * pow2_2n;
    uint64_t s                = sqrt128(d);                 /* ⌊√d⌋ = ⌊√5·2^n⌋ */

    unsigned __int128 numerator = (unsigned __int128)3 * pow2n + s + 1;
    return (seq_t)(numerator >> 1);  /* divide by 2 */
}

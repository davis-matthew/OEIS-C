/*
 * A381707  Smallest initial value for unimodal Collatz (3x+1)/2 glide
 *          sequence that begins with exactly n increases.
 * https://oeis.org/A381707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * (3^(-n) mod 2^max(2, ceil(log2(3^n)-n))) - 1 *
 *            (unimodal Collatz glide sequence with exactly n increases). *
 *            The value is computed using modular inversion modulo a power of
 *            two, * where the modulus exponent is * m = max(2,
 *            floor(n*log2(3)) + 1 - n )
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   bitlen_seq_t, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t) */
#include "../utils/bitlen_seq_t.h" /* provides unsigned bitlen_seq_t(seq_t) */

static const long double LOG2_3 = 1.5849625007211560907679L;

/* Computes a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not used, but safe */

    /* ----- 1. Compute the modulus exponent m  ----- */
    /* m = max(2, floor(n*log2(3)) + 1 - n)  */
    unsigned long long t = (unsigned long long)
        (floorl( (long double)n * LOG2_3 ) + 1 - n);
    idx_t m = (idx_t)t;
    if (m < 2) m = 2;

    /* ----- 2. Compute 2^m - 1 as the modulus mask  ----- */
    seq_t modmask;
    /* For builds where m may exceed the built‑in width (e.g. 64‑bit) */
    if (m >= ((idx_t)sizeof(seq_t) * 8)) {
        /* all bits 1 (truncated to seq_t width) */
        modmask = (seq_t)~(seq_t)0;
    } else {
        modmask = pow2(m) - 1;
    }

    /* ----- 3. Compute a_mod = 3^n (mod 2^m)  ----- */
    seq_t result = 1;
    seq_t base   = 3;
    idx_t exp    = n;
    while (exp) {
        if (exp & 1) result = (result * base) & modmask;
        base = (base * base) & modmask;
        exp >>= 1;
    }
    seq_t a_mod = result;  /* 3^n mod 2^m */

    /* ----- 4. Compute inverse of a_mod mod 2^m (Newton iteration) ----- */
    seq_t inv  = 1;
    seq_t mask = 1;   /* 2^1 - 1 */
    for (idx_t i = 1; i < m; ++i) {
        mask = (mask << 1) | 1;                /* 2^i - 1 */
        seq_t t = (a_mod * inv) & mask;
        seq_t factor = ( (seq_t)2 - t ) & mask;/* 2 - a_mod*inv mod 2^i */
        inv = (inv * factor) & mask;
    }

    /* ----- 5. Combine to get a(n) = 2^n * inv - 1 ----- */
    seq_t pow2_n = pow2(n);
    seq_t ans = (pow2_n * inv) - 1;
    return ans;
}

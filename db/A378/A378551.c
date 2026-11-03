/*
 * A378551  a(n) = Sum_{k=0..n} 4^k * binomial(n/2+k-1,k) * binomial(n-1,n-k).
 * https://oeis.org/A378551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 4^k * binomial(n/2+k-1,k) *
 *            binomial(n-1,n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

/* Multiply a seq_t by a 64‑bit integer using 128‑bit intermediate (if available) */
static inline seq_t mul128_seq(seq_t a, uint64_t b) {
#ifdef __SIZEOF_INT128__
    unsigned __int128 prod = (unsigned __int128)(unsigned long long)a *
                            (unsigned __int128)b;
    return (seq_t)prod;            /* cast back – product fits in seq_t */
#else
    return a * (seq_t)b;           /* available only when seq_t is 64‑bit */
#endif
}

/* Euclidean GCD for 64‑bit integers */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* first non‑zero term (k = 1) */
    seq_t term = (seq_t)2 * (seq_t)n;
    seq_t sum  = term;

    for (idx_t k = 1; k < n; ++k) {
        /* ratio factors */
        uint64_t num = 2ULL * (n + 2 * k) * (n - k);
        uint64_t den = k * (k + 1);

        /* cancel common factors between num and den first */
        uint64_t g = gcd_u64(num, den);
        num /= g;
        den /= g;

        /* divide current term by the remaining denominator (exact) */
        term = term / (seq_t)den;

        /* multiply by the remaining numerator */
        term = mul128_seq(term, num);

        sum += term;
    }

    return sum;
}

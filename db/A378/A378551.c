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
 * Helpers:   gcd_uint64, mul128_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_uint64.h"
#include "../utils/mul128_seq.h"

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
        uint64_t g = gcd_uint64(num, den);
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

/*
 * A382044  E.g.f. A(x) satisfies A(x) = 1 + x*A(x)^4*exp(2*x*A(x)).
 * https://oeis.org/A382044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A382044
   a(n) = n! * Sum_{k=0..n} (2*k)^(n-k) * binomial(n+3*k+1,k)
/          --------------------------------------   (n+3*k+1)*(n-k)!.
   Implementation uses the definition directly; it works with
   integer types via 128‑bit intermediate arithmetic where
   available, and with arbitrary‑precision types via the
   provided seq_t operations.
*/
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/gcd.h"

static inline seq_t mul3div(seq_t a, seq_t b, seq_t c, seq_t d)
{
#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
    __uint128_t temp = (__uint128_t)a * b;
    temp = temp * c;
    return (seq_t)(temp / d);
#else
    seq_t g;
    g = gcd(a, d); a /= g; d /= g;
    g = gcd(b, d); b /= g; d /= g;
    g = gcd(c, d); c /= g; d /= g;
    /* after division d should be 1 */
    return a * b * c;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    seq_t f = 1; /* falling factorial (n)_k */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t power = big_pow((seq_t)(2 * k), (seq_t)(n - k));
        seq_t B = binomial((idx_t)(n + 3 * k + 1), k);
        seq_t denom = (seq_t)(n + 3 * k + 1);

        seq_t term = mul3div(f, power, B, denom);
        sum += term;

        if (k < n)
            f *= (seq_t)(n - k);
    }
    return sum;
}

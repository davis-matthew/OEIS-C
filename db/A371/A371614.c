/*
 * A371614  G.f. satisfies A(x) = ( 1 + x / (1 - x*A(x)^2)^2 )^2.
 * https://oeis.org/A371614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A371614
   Formula: a(n) = Sum_{k=0..n}
          binomial(4*(n-k)+2, k) * binomial(n+k-1, n-k)/(2*(n-k)+1) */
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

static inline seq_t mul_div(seq_t a, seq_t b, seq_t d)
{
#if defined(__SIZEOF_INT128__)
    /* Safe multiplication using 128‑bit intermediate */
    __uint128_t prod = (__uint128_t)a * (__uint128_t)b;
    return (seq_t)(prod / d);
#else
    /* Fallback: split the division between the two factors */
    seq_t g = gcd(a, d);
    a /= g;          d /= g;
    g = gcd(b, d);
    b /= g;          d /= g;
    return a * b / d;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0)        /* manually handle the n = 0 case to avoid binomial(-1,0) */
        return 1;

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t nmk = n - k;                  /* n-k */
        idx_t idx1 = 4 * nmk + 2;           /* 4*(n-k)+2 */
        idx_t idx2 = n + k - 1;             /* n+k-1 */

        seq_t bin1 = binomial(idx1, k);
        seq_t bin2 = binomial(idx2, nmk);
        seq_t denom = 2 * nmk + 1;

        seq_t term = mul_div(bin1, bin2, denom);
        sum += term;
    }
    return sum;
}

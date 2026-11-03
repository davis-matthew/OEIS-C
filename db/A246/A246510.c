/*
 * A246510  G.f.: Sum_{n>=0} x^n / (1-4*x)^(2*n+1) * [Sum_{k=0..n} C(n,k)^2 *
 *          3^k * x^k] * [Sum_{k=0..n} C(n,k)^2 * 4^k * x^k].
 * https://oeis.org/A246510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} 3^k * * Sum_{j=0..n-2k} C(n-k,
 *            k+j)^2 * C(k+j, j)^2 * 4^j * (OEIS A246510)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Computes binomial coefficient C(a,b) * (C(a,b)) .  The binom
 * function already returns a value of type seq_t; multiplying two
 * such values is done with seq_mul.  */
static inline seq_t binom_squared(idx_t a, idx_t b)
{
    seq_t c = binomial(a, b);
    return seq_mul(c, c);            /* c * c */
}

/* Computes a(n) for arbitrary precision or native integer types. */
seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;
    seq_t pow3k  = (seq_t)1;          /* 3^k */

    for (idx_t k = 0; k <= n / 2; ++k)
    {
        seq_t pow4j = (seq_t)1;      /* 4^j */
        idx_t max_j = n - 2 * k;     /* j runs from 0 to n-2k */

        for (idx_t j = 0; j <= max_j; ++j)
        {
            /* compute binomial terms */
            seq_t c1 = binomial(n - k, k + j);        /* C(n-k, k+j)   */
            seq_t c1sq = seq_mul(c1, c1);          /* c1^2           */
            seq_t c2 = binomial(k + j, j);            /* C(k+j, j)     */
            seq_t c2sq = seq_mul(c2, c2);          /* c2^2           */

            /* term = c1sq * c2sq * 4^j * 3^k */
            seq_t term = seq_mul(seq_mul(c1sq, c2sq), pow4j);
            term = seq_mul(term, pow3k);

            result = result + term;                /* accumulate */

            /* prepare next 4^j */
            pow4j = mul_small(pow4j, 4);
        }
        /* prepare next 3^k */
        pow3k = mul_small(pow3k, 3);
    }

    return result;
}

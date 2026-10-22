/*
 * A346683  a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(7*k,k) / (6*k + 1).
 * https://oeis.org/A346683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(7*k, k) / (6*k + 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Greatest common divisor for unsigned __int128 */
static unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b != 0) {
        unsigned __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute binomial(7*k, k)/(6*k+1) using 128‑bit arithmetic (safe for k up to ~17) */
static seq_t small_term(uint64_t k)
{
    unsigned __int128 res = 1;
    for (uint64_t i = 1; i <= k; ++i) {
        /* Reduce res and i to keep the quotient integral */
        unsigned __int128 g = gcd128(res, i);
        res /= g;
        uint64_t i_red = i / g;

        /* Multiply by numerator factor */
        res = res * (6 * k + i);

        /* Divide by remaining part of i */
        if (i_red != 1)
            res /= i_red;
    }
    /* Final division by (6*k+1) */
    res /= (6 * k + 1);
    return (seq_t)res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t pos = 0;
    seq_t neg = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term;
        /* For small integer types use safe 128‑bit computation */
        if (sizeof(seq_t) <= 8) {
            term = small_term((uint64_t)k);
        } else {
            /* For arbitrary‑precision seq_t use the provided binomial routine */
            term = binomial(7 * k, k);
            term = term / ((seq_t)(6 * k + 1));
        }

        /* Add or subtract according to the sign (-1)^(n-k) */
        if (((n - k) & 1) != 0) {
            neg += term;
        } else {
            pos += term;
        }
    }

    /* pos >= neg for all n, so subtraction is safe */
    return pos - neg;
}

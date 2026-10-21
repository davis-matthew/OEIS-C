/*
 * A255811  Rectangular array: row n gives the numerators in the positive
 *          convolutory n-th root of (1,1,1,...).
 * https://oeis.org/A255811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl, frac
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 48, u32 n <= 125, u64 n <= 281,
 *            u128 n <= 708, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Formula: a(n) is the numerator of the reduced fraction for the k‑th
 * coefficient of (1 - t)^(-1/r) where r and k are obtained by
 * anti‑diagonal enumeration of the array of numerators:
 *   let s be the smallest integer with (s+1)(s+2)/2 ≥ n,
 *   let pos = n - s(s+1)/2 - 1  (0‑based index within diagonal),
 *   then r = pos + 1  and  k = s - pos.
 * The coefficient equals
 *   prod_{j=0}^{k-1} (1 + r*j) / (r^k * k!) reduced to lowest terms.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Compute numerator for row r (≥1) and column k (≥0) */
static seq_t numerator_row_col(idx_t r, idx_t k)
{
    if (k == 0)
        return (seq_t)1;

    seq_t num = (seq_t)1;
    seq_t den = (seq_t)1;

    for (idx_t j = 0; j < k; ++j)
    {
        seq_t a = (seq_t)1 + (seq_t)r * j;          /* numerator factor */
        seq_t b = (seq_t)r * (j + 1);              /* denominator factor */

        /* Cancel common factors with current denominator */
        seq_t g1 = gcd(a, den);
        a /= g1;
        den /= g1;

        /* Cancel common factors with current numerator */
        seq_t g2 = gcd(num, b);
        num /= g2;
        b /= g2;

        /* Multiply reduced factors */
        num *= a;
        den *= b;

        /* Final reduction to keep numbers small */
        seq_t g3 = gcd(num, den);
        num /= g3;
        den /= g3;
    }

    /* The sequence value is the numerator of the reduced fraction */
    return num;
}

seq_t a(idx_t n)
{
    /* Find diagonal s such that (s+1)(s+2)/2 ≥ n */
    idx_t s = 0;
    while ((unsigned __int128)(s + 1) * (s + 2) / 2 < (unsigned __int128)n)
        ++s;

    /* Number of terms before diagonal s */
    unsigned __int128 tprev = (unsigned __int128)s * (s + 1) / 2;
    idx_t pos = n - (idx_t)tprev - 1;   /* 0‑based within diagonal */

    idx_t r = pos + 1;
    idx_t k = s - pos;

    return numerator_row_col(r, k);
}

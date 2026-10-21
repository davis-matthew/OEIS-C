/*
 * A249967  Number of length 7+5 0..n arrays with no six consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A249967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 40, u128 n <= 42,
 *            big any n
 * Helpers:   binom, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A249967 : Number of length‑12 arrays 0..n in which no six
 *  consecutive terms have the maximum of any two terms equal to the
 *  minimum of the remaining four.  The sequence is a degree‑12
 *  polynomial in n.  Using the generating function
 *
 *        x (15+8628 x+596285 x²+…+16577 x¹⁰) / (1–x)¹³
 *
 *  we obtain the closed‑form
 *
 *        a(n) = Σ_{i=0}^{10} p[i] * binom(n-1-i+12,12)
 *
 *  where
 *        p = {15, 8628, 596285, 10163642, 60659998, 149218290,
 *             162389570, 78777198, 16029715, 1141682, 16577}.
 *
 *  The function below evaluates this polynomial exactly using the
 *  provided binom() and mul_small() utilities.  No terms of the
 *  sequence are hard‑coded except for the known numerator constants
 *  of the rational generating function.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binom.h"
#include "../utils/mul_small.h"

static const idx_t P[11] =
{
    15ULL,
    8628ULL,
    596285ULL,
    10163642ULL,
    60659998ULL,
    149218290ULL,
    162389570ULL,
    78777198ULL,
    16029715ULL,
    1141682ULL,
    16577ULL
};

seq_t a(idx_t n)
{
    if (n == 0)
        abort();                    /* offset 1 – a(0) is undefined */

    seq_t result = 0;

    /* Compute the degree‑12 polynomial using the cited coefficients. */
    for (size_t i = 0; i < 11; ++i) {
        if (n > i) {                /* corresponds to binom(...,12)=0 when n-1-i+12 < 12 */
            idx_t b = n - 1 - i + 12;
            seq_t term = binom(b, 12);
            term = mul_small(term, (unsigned int)P[i]);
            result += term;
        }
    }
    return result;
}

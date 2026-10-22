/*
 * A391301  Expansion of g^2/(1 + x^2*g^4), where g = 1+x*g^3 is the g.f. of
 *          A001764.
 * https://oeis.org/A391301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 48,
 *            big any n
 * Helpers:   binomial, binomial128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A391301
   Formula: a(n) = (1/(n+1)) * sum_{k=0..⌊n/2⌋} (-1)^k * (2*k+1) *
              binomial(3*n-2*k+1, n-2*k)  */
#include <stdint.h>
#include <stddef.h>

#include "../utils/binomial.h"
#include "../utils/binomial128.h"

seq_t a(idx_t n)
{
    /* Base case is covered by the general formula, but an explicit
       check keeps the compiler from generating a loop that
       iterates zero times (kmax == 0). */
    if (n == 0u) {
        return (seq_t)1;
    }

    const idx_t kmax = n >> 1;          /* floor(n/2) */
    enum { IS_SMALL = (sizeof(seq_t) <= 8) };

    if (IS_SMALL) {
        /* All computations are performed in signed 128‑bit
           integers; the final result is cast to seq_t. */
        __int128 sum = 0;
        for (idx_t k = 0; k <= kmax; ++k) {
            idx_t   a = 3 * n - 2 * k + 1; /* upper parameter of binom */
            idx_t   b = n - 2 * k;        /* lower parameter of binom */

            __int128 bin = binomial128(a, b);
            __int128 term = bin * (__int128)(2 * k + 1);

            if (k & 1u)
                sum -= term;
            else
                sum += term;
        }
        /* sum is guaranteed to be positive for all n ≥ 0. */
        seq_t result = (seq_t)(sum / ( (__int128)n + 1 ));
        return result;
    } else {
        /* seq_t is (probably) an arbitrary‑precision type.
           All arithmetic is performed with seq_t, which can
           hold arbitrarily large values. */
        seq_t pos = 0, neg = 0;
        for (idx_t k = 0; k <= kmax; ++k) {
            idx_t   a = 3 * n - 2 * k + 1; /* upper parameter of binom */
            idx_t   b = n - 2 * k;        /* lower parameter of binom */

            seq_t bin  = binomial(a, b);
            seq_t term = bin * (seq_t)(2 * k + 1);

            if (k & 1u)
                neg += term;
            else
                pos += term;
        }
        seq_t sum = pos - neg;            /* pos ≥ neg for this sequence */
        seq_t result = sum / (seq_t)(n + 1);
        return result;
    }
}

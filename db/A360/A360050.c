/*
 * A360050  a(n) = Sum_{k=0..floor(n/4)} (-1)^k * binomial(n+3,4*k+3) *
 *          Catalan(k).
 * https://oeis.org/A360050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} (-1)^k * binomial(n+3,4*k+3) *
 *            Catalan(k)
 * Range:     i8 n <= 4, i16 n <= 11, i32 n <= 26, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    idx_t K = n / 4;               /* floor(n/4) */
    idx_t n_plus_3 = n + 3;        /* a value for binomial's first argument */

    seq_t sum = 0;                 /* accumulation variable */

    for (idx_t k = 0; k <= K; ++k) {
        idx_t choose = 4 * k + 3;  /* choose parameter for binomial */
        seq_t bin  = binomial(n_plus_3, choose);
        seq_t cat  = catalan(k);
        seq_t prod = bin * cat;

        if (k & 1)
            sum -= prod;          /* (-1)^k with k odd subtracts */
        else
            sum += prod;          /* k even adds */
    }

    return sum;
}

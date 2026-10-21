/*
 * A280845  a(n) = 16^n * n * (n!)^4.
 * https://oeis.org/A280845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16^n * n * (n!)^4
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   factorial, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* Compute (n!)^4 */
    seq_t fact = factorial(n);                /* n! */
    seq_t fact2 = seq_mul(fact, fact);        /* (n!)^2 */
    seq_t fact4 = seq_mul(fact2, fact2);      /* (n!)^4 */

    /* Compute 16^n */
    seq_t p16 = seq_pow((seq_t)16, n);

    /* Multiply together: n * 16^n * (n!)^4 */
    seq_t tmp = seq_mul(p16, fact4);
    return seq_mul(tmp, (seq_t)n);
}

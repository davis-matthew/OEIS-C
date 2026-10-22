/*
 * A307539  Heinz numbers of square integer partitions, where the Heinz number
 *          of an integer partition (y_1,...,y_k) is
 *          prime(y_1)*...*prime(y_k).
 * https://oeis.org/A307539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^n, with a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   get_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided get_prime(k) function and seq_pow(base, exp). */

#include "../utils/get_prime.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;           /* a(0) = 1 */
    uint32_t p = get_prime(n);     /* prime(n) for n ≥ 1 */
    return seq_pow((seq_t)p, n);   /* compute prime(n)^n */
}

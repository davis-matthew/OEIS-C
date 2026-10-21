/*
 * A251583  a(n) = 3^(n-1) * (n+1)^(n-3) * (n+3).
 * https://oeis.org/A251583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=5, a(n)=3^(n-1) * (n+1)^(n-3) * (n+3) for
 *            n≥3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the small cases directly */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)5;

    /* n ≥ 3, compute the product using big_pow */
    seq_t pow3 = big_pow((seq_t)3,           (seq_t)(n - 1));
    seq_t pown = big_pow((seq_t)(n + 1),     (seq_t)(n - 3));
    seq_t mult = pow3 * pown * (seq_t)(n + 3);
    return mult;
}

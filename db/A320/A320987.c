/*
 * A320987  Fibonacci rabbit sequence number n coded in base five.
 * https://oeis.org/A320987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1, a(n) = 5^(F_{n-1}) * a(n-1) + a(n-2) for n
 *            >= 2, where F_k is the k-th Fibonacci number (F_0 = 0, F_1 = 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    seq_t a0 = 0;   /* a(n-2) */
    seq_t a1 = 1;   /* a(n-1) */

    /* Fibonacci numbers: F0 = 0, F1 = 1 */
    idx_t f0 = 0;   /* F_{i-2} */
    idx_t f1 = 1;   /* F_{i-1} */

    for (idx_t i = 2; i <= n; ++i) {
        /* exponent = F_{i-1} = f1 */
        seq_t power = big_pow((seq_t)5, (seq_t)f1);   /* 5^(F_{i-1}) */
        seq_t new_a = power * a1 + a0;                /* a(i) */

        /* shift for next step */
        a0 = a1;
        a1 = new_a;

        /* advance Fibonacci numbers */
        idx_t f2 = f0 + f1;   /* next Fibonacci number F_i */
        f0 = f1;
        f1 = f2;
    }

    return a1;
}

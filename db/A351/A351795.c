/*
 * A351795  a(n) = n! * Sum_{k=0..n} (k * (n-k))^k/k!.
 * https://oeis.org/A351795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (k*(n-k))^k / k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t F   = 1;          /* current value of n! / k!   */
    for (idx_t k = n; ; --k) {
        idx_t prod = k * (n - k);
        seq_t pow_term;
        if (prod == 0 && k == 0)          /* 0^0 is treated as 1 */
            pow_term = 1;
        else
            pow_term = big_pow((seq_t)prod, (seq_t)k);

        sum += F * pow_term;

        if (k == 0) break;
        F *= k;                            /* update F to n! / (k-1)! */
    }
    return sum;
}

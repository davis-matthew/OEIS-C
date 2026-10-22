/*
 * A339378  Let n be a positive integer. For each prime divisor p of n,
 *          consider the highest power of p which does not exceed n. The sum
 *          a(n) of these powers is defined as the power-sum of n.
 * https://oeis.org/A339378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p | n} p^(⌊log_p(n)⌋)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Returns a(n) as defined in OEIS A339378 */
seq_t a(idx_t n)
{
    /* n == 0 is outside the official offset; return 0 for completeness */
    if (n < 2)
        return 0;

    idx_t orig_n = n;      /* preserve the original n */
    idx_t temp   = n;      /* will be reduced while factors are removed */
    seq_t sum    = 0;

    /* trial division up to sqrt(temp) */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            /* compute the highest power of p that does not exceed orig_n */
            seq_t pow   = (seq_t)p;
            seq_t limit = (seq_t)(orig_n / p);
            while (pow <= limit) {
                pow *= (seq_t)p;
            }
            sum += pow;

            /* strip all occurrences of p from temp */
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* if anything remains, it is a prime > sqrt(n) */
    if (temp > 1) {
        seq_t pow   = (seq_t)temp;
        seq_t limit = (seq_t)(orig_n / temp);
        while (pow <= limit) {
            pow *= (seq_t)temp;
        }
        sum += pow;
    }

    return sum;
}

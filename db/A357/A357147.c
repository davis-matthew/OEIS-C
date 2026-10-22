/*
 * A357147  a(n) = n! * Sum_{k=0..floor(n/3)} (n - 3*k)^(3*k)/(n - 3*k)!.
 * https://oeis.org/A357147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (n - 3*k)^(3*k)/(n - 3*k)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* Computes the OEIS sequence A357147.
 * Uses an iterative approach that avoids explicit factorial tables.
 * For each n, the sum is evaluated from m = n down to 0 in steps of 3,
 * maintaining the factorial ratio n! / m! incrementally. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t fact_ratio = 1;   /* n! / m! for the current m */

    for (idx_t m = n; ; ) {
        seq_t m_val = (seq_t)m;
        seq_t e_val = (seq_t)(n - m);

        /* compute m^e efficiently; handle special cases */
        seq_t pow_val;
        if (m_val == 0) {
            pow_val = (e_val == 0) ? 1 : 0;
        } else if (e_val == 0) {
            pow_val = 1;
        } else {
            pow_val = big_pow(m_val, e_val);
        }

        sum += fact_ratio * pow_val;

        if (m < 3)        /* last term reached; exit loop */
            break;

        /* update fact_ratio to n! / (m-3)! for next iteration:
         * fact_ratio_next = fact_ratio * m * (m-1) * (m-2) */
        fact_ratio = fact_ratio * m_val * (m_val - 1) * (m_val - 2);

        m -= 3;
    }
    return sum;
}

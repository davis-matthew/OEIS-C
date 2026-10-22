/*
 * A356633  a(n) = n! * Sum_{k=0..floor(n/3)} (n - 3*k)^k/6^k.
 * https://oeis.org/A356633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (n - 3*k)^k / 6^k
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   factorial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t fac = factorial(n);      /* n! */
    seq_t sum = 0;
    seq_t fact_div = fac;          /* n! / 6^k, initially k = 0 */

    for (idx_t k = 0; k <= n / 3; ++k) {
        seq_t base = (seq_t)(n - 3 * k);
        seq_t pow_base = pow_seq(base, (int)k);
        seq_t term = fact_div * pow_base;
        sum += term;

        /* Prepare divisor for next k; exact because 6^k divides n! */
        fact_div = fact_div / 6;
    }
    return sum;
}

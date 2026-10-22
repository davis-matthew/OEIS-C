/*
 * A385241  a(n) is the least positive integer k such that the digit sum of
 *          prime(n) + k divides the concatenation of prime(n) - 1 and
 *          prime(n), where prime(n) is the n-th prime.
 * https://oeis.org/A385241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the least positive integer k such that the digit sum of
 *            * prime(n) + k divides the concatenation of prime(n)‑1 and
 *            prime(n)
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   concat_parts, digit_sum, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/concat_parts.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* get the n-th prime (1‑based) */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* concatenation of prime‑1 and prime */
    seq_t concat = concat_parts((idx_t)(p32 - 1), p32);

    /* search for the minimal k */
    for (idx_t k = 1; ; ++k) {
        seq_t sum = digit_sum(p + (seq_t)k);
        if ((concat % sum) == 0)
            return (seq_t)k;
    }
}

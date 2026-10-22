/*
 * A365958  Least k such that 8*n*k+1 is a prime.
 * https://oeis.org/A365958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k ≥ 1 such that 8·n·k + 1 is prime
 * Range:     u8 n <= 12, u16 n <= 1787, u32 n <= 1787, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 1 */
    seq_t step      = (seq_t)n * (seq_t)8; /* 8·n   */
    seq_t k         = 1;             /* current k   */
    seq_t candidate = step * k + (seq_t)1; /* 8·n·k + 1 */
    while (!is_prime_big(candidate))
    {
        ++k;                                    /* try next k   */
        candidate += step;                      /* add 8·n      */
    }
    return k;
}

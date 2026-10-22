/*
 * A382305  Consecutive internal states of the linear congruential
 *          pseudo-random number rand48 for Unix when started at 1.
 * https://oeis.org/A382305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (25214903917 * a(n-1) + 11) mod 2^48 with a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined below offset */

    static const unsigned long long MUL  = 25214903917ULL;
    static const unsigned long long INC  = 11ULL;
    static const unsigned long long MASK = ((unsigned long long)1 << 48) - 1;

    seq_t val = (seq_t)1;                 /* a(1) */
    if (n == 1) return val;

    for (idx_t i = 2; i <= n; ++i) {
        unsigned long long tmp = ((unsigned long long)val * MUL + INC) & MASK;
        val = (seq_t)tmp;                 /* truncate to seq_t */
    }
    return val;
}

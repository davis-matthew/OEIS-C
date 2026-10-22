/*
 * A392466  a(1) = 1; for n > 1, a(n) = (n-1)*(n-2)^(A000120(n-1) - 1).
 * https://oeis.org/A392466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1) * (n-2)^(popcount(n-1)-1) with a(1)=1
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 63, u64 n <= 383,
 *            u128 n <= 3583, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* popcount for uint64_t (pure C, no compiler builtin) */
static inline unsigned int popcount_u64(uint64_t x)
{
    unsigned int c = 0;
    while (x) {
        x &= x - 1;
        c++;
    }
    return c;
}

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    idx_t nm1 = n - 1;
    idx_t nm2 = n - 2;

    unsigned int pc = popcount_u64(nm1);
    idx_t exp = (pc > 0) ? (pc - 1) : 0;

    seq_t base  = (seq_t)nm2;
    seq_t power = big_pow(base, (seq_t)exp);
    seq_t result = ((seq_t)nm1) * power;

    return result;
}

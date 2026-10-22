/*
 * A384406  Consecutive internal states of the IMSL pseudo-random number
 *          generator RNUN when started with ISEED=1 and RNOPT=3.
 * https://oeis.org/A384406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(n)=397204094*a(n-1) mod 2147483647 for n>1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t mulmod(seq_t a, seq_t b, seq_t mod)
{
    return (seq_t)(((uint64_t)a * (uint64_t)b) % mod);
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    const seq_t mult = (seq_t)397204094;
    const seq_t mod  = (seq_t)2147483647;

    seq_t result = 1;
    seq_t base   = mult;
    idx_t exp    = n - 1;
    while (exp) {
        if (exp & 1)
            result = mulmod(result, base, mod);
        base = mulmod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

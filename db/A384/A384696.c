/*
 * A384696  Consecutive states of the linear congruential pseudo-random number
 *          generator Cray RANF when started at 1.
 * https://oeis.org/A384696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 44485709377909^(n-1) mod 2^48
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiplier of the Cray RANF LCG. */
#define RANF_MULTIPLIER ((seq_t)44485709377909ULL)

/* Modulo 2^48.  We use the modulus operator because it is
   defined for both fixed-width and arbitrary-precision integer
   types, and works correctly even if a*b overflows a 64-bit
   integer. */
static inline seq_t mulmod(seq_t a, seq_t b)
{
    return (a * b) % (((seq_t)1 << 48));
}

/* Computes the n-th term of A384696, with a(1) = 1.  The
   sequence is defined by a(n) = 44485709377909 * a(n-1) (mod 2^48),
   hence a(n) = 44485709377909^(n-1) (mod 2^48).  This function
   evaluates the power by fast exponentiation, which runs in
   O(log n) time. */
seq_t a(idx_t n)
{
    idx_t exp = n - 1;           /* n ≥ 1, so exp is uint64_t ≥ 0 */
    seq_t result = 1;
    seq_t base   = RANF_MULTIPLIER;

    while (exp) {
        if (exp & 1)
            result = mulmod(result, base);
        base = mulmod(base, base);
        exp >>= 1;
    }

    return result;
}

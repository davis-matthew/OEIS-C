/*
 * A384776  Consecutive states of the linear congruential pseudo-random number
 *          generator 43272750451645*s mod 2^48 when started at 1.
 * https://oeis.org/A384776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 43272750451645 * a(n-1) mod 2^48; a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned __int128 MOD48 = (((unsigned __int128)1) << 48) - 1;

/* multiply two values modulo 2^48 */
static seq_t mul_mod48(seq_t a, seq_t b)
{
    unsigned __int128 prod =
        (unsigned __int128)((a & (seq_t)MOD48)) *
        (unsigned __int128)((b & (seq_t)MOD48));
    return (seq_t)(prod & (seq_t)MOD48);
}

/* a(n) for the linear congruential generator */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    const seq_t M = (seq_t)43272750451645ULL;
    idx_t e = n - 1;
    seq_t result = (seq_t)1;
    seq_t base   = M & (seq_t)MOD48;  /* ensure 48‑bit %= */

    while (e) {
        if (e & 1) result = mul_mod48(result, base);
        base = mul_mod48(base, base);
        e >>= 1;
    }
    return result;
}

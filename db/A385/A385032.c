/*
 * A385032  Consecutive states of the linear congruential pseudo-random number
 *          generator 172*s mod 30307 when started at s=1.
 * https://oeis.org/A385032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 172 * a(n-1) mod 30307, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent explicit formula: a(n) = 172^(n-1) mod 30307 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Offset is 1; the behavior for n==0 is irrelevant */
        return (seq_t)0;
    }

    /* Fast binary exponentiation computing 172^(n-1) mod 30307 */
    const uint64_t MOD = 30307ULL;
    uint64_t exp = n - 1;            /* number of multiplications */
    uint64_t base = 172ULL;          /* current power of 172 */
    uint64_t result = 1ULL;          /* accumulates the answer */

    while (exp > 0) {
        if (exp & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1ULL;
    }

    /* The result fits in the range [0,30306] which is always representable in
       any seq_t type (including 8‑, 16‑, 32‑, 64‑bit or big integer).  */
    return (seq_t)result;
}

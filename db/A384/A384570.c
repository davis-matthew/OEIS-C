/*
 * A384570  Consecutive states of the linear congruential pseudo-random number
 *          generator (3141592621*s+1) mod 10^10 when started at s=1.
 * https://oeis.org/A384570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1, a(n) = (3141592621*a(n-1)+1) mod 10000000000 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    const unsigned long long A   = 3141592621ULL;
    const unsigned long long MOD = 10000000000ULL;
    unsigned long long cur = 1ULL;
    for (idx_t i = 2; i <= n; ++i) {
        __uint128_t t = (__uint128_t)cur * A + 1;
        cur = (unsigned long long)(t % MOD);
    }
    return (seq_t)cur;
}

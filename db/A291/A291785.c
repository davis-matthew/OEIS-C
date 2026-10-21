/*
 * A291785  Iterate the map A291784: k -> (psi(k)+phi(k))/2, starting with n,
 *          until a power of a prime (A000961) is reached, or -1 if that never
 *          happens.
 * https://oeis.org/A291785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 73, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iterates f(k) = (psi(k)+phi(k))/2 until a prime power is reached
   (f(n)=n) or the trajectory becomes unbounded.  Returns
   the reached prime power, or -1 if no prime power is reached.  */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Utilities provided by the build environment. */
#include "../utils/factor_u64.h"

/* --- helper: small power in 128‑bit intermediate --- */
static inline __int128 pow_uint128(uint64_t base, int exp)
{
    __int128 res = 1;
    for (int i = 0; i < exp; ++i)
        res *= (__int128)base;
    return res;
}

/* Test if a uint64_t number is a prime power (including 1). */
static inline bool is_prime_power_uint64(uint64_t n)
{
    if (n <= 1) return true;   /* 1 is considered a prime power for this sequence */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);
    return cnt == 1;
}

/* Compute the next value of the map f(k) = (psi(k)+phi(k))/2
   for a uint64_t input.  Overflow (result > UINT64_MAX) yields 0. */
static inline uint64_t next_map_uint64(uint64_t n)
{
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    __int128 phi = 1;
    __int128 psi = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int      e = (int)exps[i];

        __int128 powp = pow_uint128(p, e - 1);          /* p^(e-1)  */
        phi *= powp * (__int128)(p - 1);                /* phi part*/
        psi *= powp * (__int128)(p + 1);                /* psi part*/
    }

    __int128 next = (phi + psi) / 2;
    if (next > (__int128)UINT64_MAX)
        return 0;   /* indicates we cannot continue (overflow) */
    return (uint64_t)next;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)               /* index 0 is outside the offset 1 */
        return (seq_t)-1;

    uint64_t cur = (uint64_t)n;
    const int MAX_STEPS = 120000;            /* safety against infinite loops */

    for (int step = 0; step < MAX_STEPS; ++step) {
        if (is_prime_power_uint64(cur))
            return (seq_t)cur;               /* reached a prime power */

        uint64_t next = next_map_uint64(cur);
        if (next == 0)                       /* overflow / unbounded */
            break;
        cur = next;
    }
    return (seq_t)-1;
}

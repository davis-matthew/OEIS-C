/*
 * A277885  a(n) = index of the least non-unitary prime divisor of n or 0 if
 *          no such prime-divisor exists.
 * https://oeis.org/A277885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index (1‑based) of the least prime divisor of n that
 *            divides n with exponent ≥ 2. If n has no such prime divisor,
 *            a(n) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define MAX_PRIME_CAP(INIT) (((INIT) >> 1) + 1)

/* Find integer floor of sqrt for a 64‑bit unsigned integer.
   Uses binary search with 128‑bit multiplication to avoid overflow. */
static uint64_t floor_sqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = x, mid, best = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        if ((__uint128_t)mid * mid <= x) {
            best = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* Convert to native 64‑bit for calculation. */
    uint64_t nn = (uint64_t)n;

    /* Build a list of primes up to sqrt(nn). */
    uint64_t limit = floor_sqrt_u64(nn);
    if (limit < 2)
        limit = 2;                      /* at least prime 2 */

    size_t cap = MAX_PRIME_CAP(limit);
    uint64_t *primes = (uint64_t*)malloc(cap * sizeof(uint64_t));
    if (!primes)
        return 0;                       /* allocation failure, safe fallback */

    size_t pc = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        int isprime = 1;
        for (size_t j = 0; j < pc && primes[j] * primes[j] <= i; ++j) {
            if (i % primes[j] == 0) {
                isprime = 0;
                break;
            }
        }
        if (isprime)
            primes[pc++] = i;
    }

    uint64_t tmp = nn;
    seq_t answer = 0;

    for (size_t idx = 0; idx < pc; ++idx) {
        uint64_t p = primes[idx];
        if (p * p > tmp)
            break;

        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if (exp >= 2) {                /* found least non‑unitary prime */
                answer = (seq_t)(idx + 1); /* primes are 1‑based indexed */
                break;
            }
        }
    }

    free(primes);
    return answer;
}

/*
 * A366522  Largest squarefree divisor of n which is < sqrt(n), for n >= 2;
 *          a(1) = 1.
 * https://oeis.org/A366522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest squarefree divisor of n which is strictly less
 *            than sqrt(n)
 * Range:     u8 n <= 1035, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Integer square‑root: floor(sqrt(n)) for arbitrary seq_t. */
static seq_t isqrt_seq(seq_t n)
{
    /* Find high bound: smallest power of two > sqrt(n). */
    seq_t high = 1;
    while (high <= n / high) {          /* avoid overflow */
        high <<= 1;
    }
    seq_t low = high >> 1;               /* first candidate <= sqrt(n) */
    /* Binary search between low and high-1. */
    while (high - low > 1) {
        seq_t mid = low + (high - low) / 2;
        if (mid <= n / mid)              /* mid*mid <= n */
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Factor n into distinct prime factors (n fits in uint64_t). */
static void factor_distinct(uint64_t n, uint64_t *primes, size_t *cnt)
{
    *cnt = 0;
    if (n % 2 == 0) {
        primes[(*cnt)++] = 2;
        while (n % 2 == 0) n /= 2;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            primes[(*cnt)++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) primes[(*cnt)++] = n;
}

/* Depth‑first search to explore all square‑free divisors up to limit. */
static void dfs(int pos, seq_t prod, seq_t limit, seq_t *best,
                const uint64_t *primes, int cnt)
{
    if (prod > *best) *best = prod;
    for (int i = pos; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        if (p == 0 || p > limit) continue;
        if (prod > limit / p) continue; /* would exceed limit */
        dfs(i + 1, prod * p, limit, best, primes, cnt);
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;   /* a(1) = 1 */

    uint64_t n64 = (uint64_t)n;    /* n is small enough for uint64_t */

    /* Compute floor(sqrt(n)) and adjust for strict inequality. */
    seq_t limit = isqrt_seq((seq_t)n);
    if (limit != 0 && (seq_t)(n64 / (uint64_t)limit) == limit)
        limit--;                   /* n is a perfect square → use < sqrt(n) */

    /* Collection of distinct primes dividing n. */
    uint64_t primes[64];
    size_t cnt;
    factor_distinct(n64, primes, &cnt);

    seq_t best = 1;
    dfs(0, (seq_t)1, limit, &best, primes, (int)cnt);
    return best;
}

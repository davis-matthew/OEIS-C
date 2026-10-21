/*
 * A279760  Expansion of Product_{k>=1} 1/(1 - x^(prime(k)^3)).
 * https://oeis.org/A279760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into cubes of primes (OEIS
 *            A279760). Computed by the coin‑change DP algorithm with coin
 *            values p³ for all primes p. The implementation keeps a static DP
 *            table for reuse across calls. It uses only integer arithmetic
 *            and works for any allowed seq_t/idx_t types
 * Range:     u8 n <= 2055, u16 n <= 2055, u32 n <= 2055, u64 n <= 2055,
 *            u128 n <= 2055, big any n
 * Helpers:   primes_up_to
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/primes_up_to.h"

/* static DP array reused between calls */
static seq_t *dp = NULL;   /* dp[i] holds a(i) for 0 <= i < dp_len */
static size_t dp_len = 0;  /* current allocated length */

/* integer cube root: floor((n)^(1/3)) computed without floating point
   to avoid precision issues.  We use unsigned __int128 for safety. */
static idx_t cube_root(idx_t n)
{
    idx_t r = 0;
    while (true) {
        idx_t next = r + 1;
        unsigned __int128 nxt3 = (unsigned __int128)next * next * next;
        if (nxt3 > n) break;
        r = next;
    }
    return r;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* Ensure DP table covers 0..n */
    size_t needed = (size_t)n + 1;
    if (dp_len < needed) {
        seq_t *tmp = (seq_t *)realloc(dp, needed * sizeof(seq_t));
        dp   = tmp;
        dp_len = needed;
    }

    /* initialize dp[0] = 1, others = 0 */
    dp[0] = 1;
    for (size_t i = 1; i < needed; ++i) dp[i] = 0;

    /* compute all primes up to cube root of n */
    idx_t limit = cube_root(n);
    /* static array large enough for the expected maximum count */
    idx_t primes[1000];
    size_t pcnt = 0;
    primes_up_to(limit, primes, &pcnt);

    /* coin-change DP over prime cubes */
    for (size_t i = 0; i < pcnt; ++i) {
        idx_t p = primes[i];
        unsigned __int128 cup = (unsigned __int128)p * p * p;
        idx_t coin = (idx_t)cup;   /* coin value (p³) */
        if (coin > n) break;
        for (idx_t s = coin; s <= n; ++s) {
            dp[(size_t)s] += dp[(size_t)(s - coin)];
        }
    }

    return dp[(size_t)n];
}

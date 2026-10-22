/*
 * A378369  Distance between n and the least nonsquarefree number >= n.
 * https://oeis.org/A378369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1970, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns a(n) for OEIS A378369
   (distance between n and the least nonsquarefree number ≥ n). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"       // for is_prime_small
#include "../utils/_floor_isqrt_u64.h"     // for _floor_isqrt_u64
#include "../utils/build_primes_upto.h"    // for build_primes_upto

/* seq_t and idx_t are defined by the compile system. */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t primes_limit = 0;

static void ensure_primes_upto(idx_t limit)
{
    if (limit <= primes_limit) return;
    /* Rebuild the prime table up to the new limit. */
    free(primes);
    primes_cnt = 0;
    build_primes_upto(limit, &primes, &primes_cnt);
    primes_limit = limit;
}

seq_t a(idx_t n)
{
    /* The next nonsquarefree number is a multiple of some prime square.
       For any n there is a multiple of 4 within distance ≤ 3,
       so the answer is always 0, 1, 2, or 3.  We find the minimal
       distance by checking all prime squares p² ≤ n+3. */
    idx_t limit = _floor_isqrt_u64(n + 3);
    ensure_primes_upto(limit);

    seq_t best = (seq_t)4;   /* initialize with value > any possible distance */

    for (size_t i = 0; i < primes_cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t sq = p * p;          /* p², fits in seq_t because p ≤ limit */
        if (sq > (seq_t)(n + 3)) break;  /* further squares will only increase distance */

        seq_t r = (seq_t)n % sq;
        if (r == 0) return 0;              /* n itself is nonsquarefree */

        seq_t dist = sq - r;
        if (dist < best) best = dist;
        if (best == 0) break;
    }
    return best;
}

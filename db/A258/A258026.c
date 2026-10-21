/*
 * A258026  Numbers k such that prime(k+2) - 2*prime(k+1) + prime(k) < 0.
 * https://oeis.org/A258026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th index k (1‑based) such that prime(k+2) -
 *            2*prime(k+1) + prime(k) < 0. (A258026 – positions of strict
 *            descents in the prime gap sequence)
 * Range:     u8 n <= 109, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

/* static data for memoisation */
static uint64_t *g_primes      = NULL;  /* primes array (prime[0] = 2) */
static size_t   g_prime_cnt    = 0;      /* number of primes stored      */
static idx_t   *g_positions    = NULL;  /* positions k where descent occurs (1‑based) */
static size_t   g_pos_cnt      = 0;      /* number of such positions       */

/* initialise primes and descent positions once */
static void init(void)
{
    if (g_primes != NULL)
        return;                       /* already initialised */

    /* estimate a bound that contains >200 000 primes – enough for n up to 10 000 */
    idx_t limit = _prime_limit_for_idx(200000);
    build_primes_upto(limit, &g_primes, &g_prime_cnt);

    /* we need primes up to k+2, so the largest required k is g_prime_cnt-2 */
    size_t max_k = g_prime_cnt - 2;
    g_positions = (idx_t*)malloc(max_k * sizeof(idx_t));

    size_t pos_i = 0;
    for (size_t k = 1; k <= max_k; ++k) {
        /* indices: prime(k)   = g_primes[k-1]
                    prime(k+1) = g_primes[k]
                    prime(k+2) = g_primes[k+1] */
        int64_t diff = (int64_t)g_primes[k+1]
                       - 2LL * (int64_t)g_primes[k]
                       + (int64_t)g_primes[k-1];
        if (diff < 0)
            g_positions[pos_i++] = (idx_t)k;
    }
    g_pos_cnt = pos_i;
}

seq_t a(idx_t n)
{
    init();                     /* ensure data is ready */
    /* a(1) corresponds to g_positions[0] */
    return (seq_t)g_positions[n - 1];
}

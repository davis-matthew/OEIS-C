/*
 * A361615  a(n) is the smallest 5-rough number with exactly n divisors.
 * https://oeis.org/A361615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal 5‑rough number with exactly n divisors
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 16, u64 n <= 28, u128 n <= 31,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes.h"

#define MAX_N 10000

static seq_t best[MAX_N + 1] = {0};
static bool computed = false;
static idx_t *prime_array = NULL;   /* pointers returned by build_primes()      */
static idx_t *primes = NULL;        /* points to the first prime ≥ 5           */
static size_t prime_cnt = 0;        /* number of primes ≥ 5 used in search    */

/* Depth‑first search that records the smallest number for each divisor count */
static void dfs(size_t pi, unsigned int last_exp, seq_t cur_val, idx_t cur_div)
{
    if (pi >= prime_cnt) return;

    seq_t val = cur_val;
    idx_t p = primes[pi];
    for (unsigned int e = 1; e <= last_exp; ++e) {
        val *= (seq_t)p;
        idx_t new_div = cur_div * (e + 1);
        if (new_div > MAX_N) break;              /* we only need divisor counts ≤ MAX_N */
        if (best[new_div] == 0 || val < best[new_div]) best[new_div] = val;
        dfs(pi + 1, e, val, new_div);
    }
}

/* One‑time initialisation: build prime list and precompute the table */
static void init(void)
{
    if (computed) return;

    const idx_t PRIME_LIMIT = 2000;        /* large enough to contain all needed primes */
    size_t total_primes = 0;
    build_primes(PRIME_LIMIT, &prime_array, &total_primes);

    /* find first prime ≥ 5 */
    size_t offset = 0;
    while (offset < total_primes && prime_array[offset] < 5U) ++offset;

    primes = prime_array + offset;
    prime_cnt = total_primes - offset;

    best[1] = 1;                           /* a(1) = 1 */
    dfs(0, MAX_N - 1, (seq_t)1, 1);

    computed = true;
}

seq_t a(idx_t n)
{
    init();
    return best[(size_t)n];
}

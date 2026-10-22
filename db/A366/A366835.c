/*
 * A366835  In the pair (A246655(n), A246655(n+1)), how many primes are there?
 * https://oeis.org/A366835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes among A246655(n) and A246655(n+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

static idx_t *prime_powers = NULL;   /* ascending list of prime powers */
static size_t pp_count     = 0;      /* number of stored values   */
static idx_t   pp_limit    = 0;      /* largest value generated   */

/* Comparator for qsort on idx_t values */
static int cmp_idx(const void *a, const void *b)
{
    idx_t v1 = *(const idx_t *)a;
    idx_t v2 = *(const idx_t *)b;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

/* Ensure that at least `needed` prime powers are present (index 0‑based). */
static void ensure_pp_up_to(idx_t needed)
{
    if (prime_powers && pp_count >= needed)
        return;

    /* Start with a small limit and double until sufficient. */
    idx_t limit = pp_limit ? pp_limit * 2 : 2;

    while (true) {
        uint64_t *primes = NULL;
        size_t    pr_cnt = 0;
        build_primes_upto(limit, &primes, &pr_cnt);

        /* Allocate temporary buffer for prime powers */
        size_t cap = pr_cnt * 4 + 32;
        idx_t *tmp = (idx_t *)malloc(cap * sizeof(idx_t));
        if (!tmp) exit(1);

        size_t cnt = 0;
        for (size_t pi = 0; pi < pr_cnt; ++pi) {
            idx_t p = (idx_t)primes[pi];
            idx_t val = p;
            while (true) {
                if (cnt == cap) {
                    cap <<= 1;
                    idx_t *nxt = (idx_t *)realloc(tmp, cap * sizeof(idx_t));
                    if (!nxt) { free(tmp); exit(1); }
                    tmp = nxt;
                }
                tmp[cnt++] = val;
                if (val > limit / p) break;
                val *= p;
            }
        }
        free(primes);

        /* Sort ascending */
        qsort(tmp, cnt, sizeof(idx_t), cmp_idx);

        /* Replace old table */
        free(prime_powers);
        prime_powers = tmp;
        pp_count     = cnt;
        pp_limit     = limit;

        if (pp_count >= needed)
            break;

        /* Increase limit for next attempt */
        limit <<= 1;
    }
}

seq_t a(idx_t n)
{
    /* n is 1‑based. Ensure we have indices n‑1 and n available. */
    ensure_pp_up_to(n + 1);

    idx_t v1 = prime_powers[n - 1];
    idx_t v2 = prime_powers[n];

    seq_t result = 0;
    if (is_prime(v1)) result += 1;
    if (is_prime(v2)) result += 1;
    return result;
}

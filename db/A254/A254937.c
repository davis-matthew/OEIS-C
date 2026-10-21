/*
 * A254937  Fundamental positive solution y = y2(n) of the second class of the
 *          Pell equation x^2 - 2*y^2 = -A007519(n), n>=1 (primes congruent to
 *          1 mod 8).
 * https://oeis.org/A254937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 74, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   enlarge_seq_t, enlarge_uint32, floor_sqrt, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the second smallest positive y satisfying
   x^2 - 2*y^2 = -p, where p is the n-th prime ≡ 1 (mod 8). */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/floor_sqrt.h"
#include "../utils/enlarge_seq_t.h"
#include "../utils/enlarge_uint32.h"

/* static state ------------------------------------------------------------ */
static bool initialized = false;
static uint32_t *good_primes = NULL;     /* primes ≡ 1 (mod 8)          */
static size_t gp_cap = 0, gp_cnt = 0;    /* capacity and count         */
static seq_t *cache = NULL;             /* cached y values            */
static size_t cache_cap = 0, cache_cnt = 0;
static uint32_t last_checked = 0;       /* last candidate processed   */

/* compute the second positive solution y for a given prime p (p ≡ 1 (mod 8)) */
static seq_t second_solution(uint32_t p)
{
    seq_t y = 1;
    bool first_found = false;
    while (1) {
        seq_t lhs = (seq_t)2 * y * y;
        if (lhs < (seq_t)p) {              /* 2*y^2 < p → negative RHS */
            ++y;
            continue;
        }
        seq_t t = lhs - (seq_t)p;           /* t = 2*y^2 - p, now ≥ 0   */
        seq_t sq = floor_sqrt(t);
        if (sq * sq == t) {
            if (!first_found) {
                first_found = true;        /* first (smallest) solution */
            } else {
                return y;                  /* second smallest solution  */
            }
        }
        ++y;
    }
}

/* ensure we have at least 'need' primes ≡ 1 (mod 8) in good_primes -------- */
static void ensure_primes(size_t need)
{
    while (gp_cnt < need) {
        /* find next prime ≡ 1 (mod 8) */
        uint32_t cand = (last_checked / 8 + 1) * 8 + 1;   /* next 1 mod 8 */
        while (!is_prime_small((seq_t)cand)) cand += 8;
        enlarge_uint32(&good_primes, &gp_cap, gp_cnt + 1);
        good_primes[gp_cnt++] = cand;
        last_checked = cand;
    }
}

/* ensure cache has values up to index 'need' (1‑based) --------------------- */
static void ensure_cache(size_t need)
{
    if (cache_cnt >= need) return;
    ensure_primes(need);                       /* primes up to needed index */
    enlarge_seq_t(&cache, &cache_cap, need);
    for (size_t i = cache_cnt; i < need; ++i) {
        cache[i] = second_solution(good_primes[i]);   /* 0‑based array */
    }
    cache_cnt = need;
}

/* public sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1 – unused for n=0 */
    ensure_cache((size_t)n);
    return cache[(size_t)n - 1];
}

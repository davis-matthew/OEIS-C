/*
 * A363949  Numbers whose prime indices have mean 1 when rounded down.
 * https://oeis.org/A363949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * A344296(n)
 * Range:     u8 n <= 35, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The terms are the numbers 2*m such that the average of the prime indices
    of 2*m (rounded down) equals 1.  Equivalently, for m ≥ 1 let
    S = 1 + Σ e_p * idx(p)   (index of 2 is 1)
    T = 1 + Σ e_p
    then 2*m is a term iff S < 2*T.                                 */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- global tables for prime factorisation ---------- */
static idx_t *g_spf          = NULL;    /* smallest prime factor for each number */
static uint32_t *g_prime_idx = NULL;    /* index of prime p, 1‑based */
static idx_t *g_primes       = NULL;    /* list of primes */
static size_t g_primes_cap   = 16;      /* current capacity of g_primes */
static size_t g_primes_cnt   = 0;       /* number of primes stored */
static idx_t   g_spf_cap     = 0;       /* maximum n for which tables are built */

/* build (or rebuild) tables up to limit.  Rebuilding is performed on
   demand and uses a simple linear sieve, which is fast enough for the
   sizes required by this sequence. */
static void ensure_sieve(idx_t limit)
{
    if (limit <= g_spf_cap) return;

    /* resize tables */
    size_t cap = (size_t)limit;
    g_spf      = (idx_t *)realloc(g_spf,      (cap + 1) * sizeof(idx_t));
    g_prime_idx = (uint32_t *)realloc(g_prime_idx,
                                      (cap + 1) * sizeof(uint32_t));
    if (g_primes == NULL) {
        g_primes = (idx_t *)malloc(g_primes_cap * sizeof(idx_t));
    }

    for (size_t i = 0; i <= cap; ++i) {
        g_spf[i]      = 0;
        g_prime_idx[i] = 0;
    }

    size_t cnt = 0;                    /* number of primes found so far */
    for (idx_t i = 2; i <= limit; ++i) {
        if (g_spf[i] == 0) {           /* i is prime */
            g_spf[i] = i;
            if (cnt >= g_primes_cap) {
                g_primes_cap *= 2;
                g_primes = (idx_t *)realloc(g_primes,
                                            g_primes_cap * sizeof(idx_t));
            }
            g_primes[cnt] = i;
            g_prime_idx[i] = (uint32_t)(cnt + 1); /* 1‑based index */
            ++cnt;
        }
        for (size_t j = 0; j < cnt; ++j) {
            idx_t p  = g_primes[j];
            idx_t ip = i * p;
            if (ip > limit) break;
            g_spf[ip] = p;
            if (p == g_spf[i]) break;
        }
    }

    g_primes_cnt = cnt;
    g_spf_cap    = limit;
}

/* test whether 2*m satisfies the mean‑1 condition. */
static bool valid_candidate(idx_t m)
{
    unsigned __int128 sum    = 1;   /* index of prime 2 */
    unsigned __int128 total  = 1;   /* exponent of prime 2 */

    idx_t x = m;
    while (x > 1) {
        idx_t p  = g_spf[x];
        idx_t e  = 0;
        while (x % p == 0) { x /= p; ++e; }
        sum   += (unsigned __int128)e * (unsigned __int128)g_prime_idx[p];
        total += e;
    }

    return sum < (unsigned __int128)2 * total;
}

/* ---------- memoised sequence generator ---------- */
static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static idx_t next_m      = 1;      /* next m to test */

seq_t a(idx_t n)
{
    /* fill cache until we have at least n terms */
    while (cache_size < n) {
        ensure_sieve(next_m);             /* make sure we can factor next_m */
        if (valid_candidate(next_m)) {
            seq_t val = (seq_t)next_m;    /* 2 * m */
            val <<= 1;
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_size++] = val;
        }
        ++next_m;
    }
    return cache[n - 1];
}

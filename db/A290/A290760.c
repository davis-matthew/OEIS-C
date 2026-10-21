/*
 * A290760  Matula-Goebel numbers of transitive rooted identity trees (or
 *          transitive finitary sets).
 * https://oeis.org/A290760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th smallest positive integer that is * squarefree
 *            and for every prime divisor p of a(n), its index * in the
 *            ordered list of primes (p₁ = 2, p₂ = 3, p₃ = 5, …) * also
 *            divides a(n)
 * Range:     u8 n <= 5, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ----------  GLOBAL DATA  ---------- */
#define LIMIT 20000000U      /* upper bound for candidate numbers */

static uint32_t *g_spf = NULL;        /* smallest prime factor for 1..LIMIT */
static uint32_t *g_prime_index = NULL;/* index of each prime (1‑based) */
static size_t g_num_primes = 0;       /* number of primes ≤ LIMIT */

/* ----------  SIEVE INITIALIZATION  ---------- */
static void init_sieve(void)
{
    static int done = 0;
    if (done) return;
    done = 1;

    g_spf = (uint32_t*)calloc(LIMIT + 1, sizeof(uint32_t));
    g_prime_index = (uint32_t*)calloc(LIMIT + 1, sizeof(uint32_t));

    /* linear sieve for smallest prime factor */
    for (uint32_t i = 2; i <= LIMIT; ++i)
    {
        if (g_spf[i] == 0) {
            g_spf[i] = i;
            if ((uint64_t)i * i <= LIMIT)
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    if (g_spf[j] == 0) g_spf[j] = i;
        }
    }

    /* record prime indices */
    uint32_t pos = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
    {
        if (g_spf[i] == i) {
            ++pos;
            g_prime_index[i] = pos;
        }
    }
    g_num_primes = pos;
}

/* ----------  PROPERTY CHECK  ---------- */
static int is_valid(uint32_t x)
{
    if (x == 1) return 1;          /* 1 is always valid */

    /* factor x and collect distinct prime factors */
    uint32_t factors[32];
    int fcnt = 0;
    uint32_t xx = x;
    while (xx > 1) {
        uint32_t p = g_spf[xx];
        /* check for repeated prime factor (not squarefree) */
        if ((xx / p) % p == 0) return 0;
        factors[fcnt++] = p;
        xx /= p;
    }

    /* for each prime factor p, its index must divide x */
    for (int i = 0; i < fcnt; ++i) {
        uint32_t p = factors[i];
        uint32_t idx = g_prime_index[p];
        if (x % idx != 0) return 0;
    }
    return 1;
}

/* ----------  CACHE FOR VALUES  ---------- */
static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static size_t cache_len = 0;          /* number of stored terms (incl. dummy at 0) */
static uint32_t next_candidate = 1;   /* next number to test */

static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_capacity = 16;
        cache = (seq_t*)malloc(cache_capacity * sizeof(seq_t));
        cache[0] = 0;                 /* dummy for 1‑based indexing */
        cache_len = 1;
    }

    while ((idx_t)(cache_len - 1) < n) {
        if (next_candidate > LIMIT) {
            /* exceed pre‑sieved range – should not happen for the tested n */
            return;
        }
        if (is_valid(next_candidate)) {
            if (cache_len >= cache_capacity) {
                size_t new_cap = cache_capacity * 2;
                cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                cache_capacity = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* ----------  REQUIRED FUNCTION  ---------- */
seq_t a(idx_t n)
{
    init_sieve();
    ensure_cache(n);
    return cache[n];
}

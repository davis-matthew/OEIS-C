/*
 * A304652  Digits of the Copeland-Erdős constant taken in groups of five
 *          digits.
 * https://oeis.org/A304652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the 5‑digit sliding window starting at position n of the
 *            decimal expansion of the Copeland‑Erdős constant
 * Range:     u8 none, u16 n <= 3, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Global state for digits of the Copeland–Erdős constant                */
/* --------------------------------------------------------------------- */
static unsigned char *g_digits   = NULL;          /* buffer holding the concatenated prime digits */
static size_t          g_digits_cap = 0;
static size_t          g_digits_len = 0;

/* Global state for prime generation                                    */
static uint64_t *g_primes   = NULL;                /* list of known primes */
static size_t   g_prime_cap = 0;
static size_t   g_prime_cnt = 0;
static uint64_t g_next_candidate = 2;

/* Global cache for computed sequence values                           */
static seq_t *g_cache    = NULL;                   /* cached 5‑digit values */
static size_t g_cache_cap = 0;
static size_t g_cache_len = 0;

/* --------------------------------------------------------------------- */
/* Helper functions                                                      */
/* --------------------------------------------------------------------- */

static void ensure_digits_capacity(size_t needed)
{
    if (g_digits_cap < needed) {
        size_t new_cap = g_digits_cap ? g_digits_cap << 1 : 1024;
        if (new_cap < needed) new_cap = needed;
        g_digits = (unsigned char *)realloc(g_digits, new_cap);
        g_digits_cap = new_cap;
    }
}

static void ensure_prime_capacity(size_t needed)
{
    if (g_prime_cap < needed) {
        size_t new_cap = g_prime_cap ? g_prime_cap << 1 : 64;
        if (new_cap < needed) new_cap = needed;
        g_primes = (uint64_t *)realloc(g_primes, new_cap * sizeof(uint64_t));
        g_prime_cap = new_cap;
    }
}

static void ensure_cache_capacity(size_t needed)
{
    if (g_cache_cap < needed) {
        size_t new_cap = g_cache_cap ? g_cache_cap << 1 : 1024;
        if (new_cap < needed) new_cap = needed;
        g_cache = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
        g_cache_cap = new_cap;
    }
}

static void emit_prime_digits(uint64_t p)
{
    char tmp[32];
    int pos = 0;
    do {
        tmp[pos++] = (char)('0' + (p % 10));
        p /= 10;
    } while (p);
    for (int i = pos - 1; i >= 0; --i)
        g_digits[g_digits_len++] = (unsigned char)(tmp[i] - '0');
}

static uint64_t next_prime(void)
{
    for (;;) {
        uint64_t cand = g_next_candidate++;
        if (cand == 2) {
            ensure_prime_capacity(g_prime_cnt + 1);
            g_primes[g_prime_cnt++] = 2;
            return 2;
        }
        if ((cand & 1) == 0) continue;

        bool is_prime = true;
        for (size_t i = 0; i < g_prime_cnt; ++i) {
            uint64_t p = g_primes[i];
            if (p > cand / p) break;   /* p*p > cand */
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            ensure_prime_capacity(g_prime_cnt + 1);
            g_primes[g_prime_cnt++] = cand;
            return cand;
        }
    }
}

/* Ensure that at least (n+4) prime digits are available in g_digits.   */
static void generate_digits_up_to(idx_t n)
{
    if (g_digits_len >= (size_t)n + 4) return;
    while (g_digits_len < (size_t)n + 4) {
        uint64_t p = next_prime();
        ensure_digits_capacity(g_digits_len + 6); /* a few extra slots for the prime */
        emit_prime_digits(p);
    }
}

/* Compute the value a(k) for all k up to n.  The value is stored in the cache. */
static seq_t compute_value(idx_t n)
{
    if (g_cache_len >= (size_t)n) return g_cache[n - 1];

    for (size_t k = g_cache_len + 1; k <= (size_t)n; ++k) {
        uint64_t val64 = 0;
        for (int d = 0; d < 5; ++d)
            val64 = val64 * 10 + (uint64_t)g_digits[(k - 1) + d];
        seq_t val = (seq_t)val64;
        ensure_cache_capacity(g_cache_len + 1);
        g_cache[g_cache_len++] = val;
    }
    return g_cache[n - 1];
}

/* --------------------------------------------------------------------- */
/* Public sequence function                                             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety guard – not used for OEIS offsets */
    generate_digits_up_to(n);
    return compute_value(n);
}

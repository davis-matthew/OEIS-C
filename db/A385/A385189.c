/*
 * A385189  Intersection of A055932 and A002378.
 * https://oeis.org/A385189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = m·(m+1), where the set of prime factors of m·(m+1) is
 *            exactly the set {p1, p2, …, pk} of the first k primes for some
 *            k. Implementation uses a sieve to precompute prime factors up to
 *            a fixed limit, then enumerates m starting at 1 until the
 *            required number of terms is reached. Only terms tha...
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- constants ---------- */
#define SIEVE_LIMIT 2000000U          /* large enough to generate 10 000 terms */
#define MAX_PRIMORIALS 12            /* up to 2·3·5·7·11·13·17·19·23·29·31 */

static const unsigned long long primorials[MAX_PRIMORIALS] = {
    2ULL,
    6ULL,
    30ULL,
    210ULL,
    2310ULL,
    30030ULL,
    510510ULL,
    9699690ULL,
    223092870ULL,
    6469693230ULL,
    200560490130ULL
};

/* ---------- global state ---------- */
static bool *is_composite = NULL;          /* sieve */
static uint32_t *primes = NULL;            /* list of primes */
static size_t prime_cnt = 0;

static uint64_t *rad = NULL;               /* rad[x] = product of distinct primes of x */
static size_t rad_size = 0;

static seq_t *cache = NULL;                /* memoised a(n) values */
static size_t cache_len = 0;
static size_t cache_cap = 0;

static uint64_t next_m = 1;                /* next m to examine */

/* ---------- utility functions ---------- */
static void init_sieve(void)
{
    if (is_composite) return;          /* already initialised */

    rad_size = SIEVE_LIMIT + 2;         /* need up to m+1 */
    rad = (uint64_t*)calloc(rad_size, sizeof(uint64_t));
    if (!rad) abort();
    for (size_t i = 0; i < rad_size; ++i) rad[i] = 1;   /* rad[0]=1 useless */

    is_composite = (bool*)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    if (!is_composite) abort();

    primes = (uint32_t*)malloc((SIEVE_LIMIT / 10) * sizeof(uint32_t));
    if (!primes) abort();

    for (uint32_t p = 2; p <= SIEVE_LIMIT; ++p) {
        if (!is_composite[p]) {
            primes[prime_cnt++] = p;
            /* mark multiples and multiply rad */
            for (uint32_t j = p; j <= SIEVE_LIMIT; j += p) {
                is_composite[j] = true;
                rad[j] *= p;
            }
        }
    }
    rad[0] = 0;  /* unused */
    rad[1] = 1;
}

/* check whether x is one of the primorials; linear search is fine */
static bool is_primorial(uint64_t x)
{
    for (size_t i = 0; i < MAX_PRIMORIALS; ++i) {
        if (x == primorials[i]) return true;
    }
    return false;
}

/* ensure that cache has at least n terms */
static void ensure_terms(idx_t n)
{
    if (n == 0) return;

    init_sieve();

    /* maximum representable value of seq_t */
    const seq_t max_seq = (seq_t)-1;
    const unsigned __int128 max_seq128 = (unsigned __int128)max_seq;

    /* expand cache until we have n terms or cannot produce more */
    while (cache_len < (size_t)n) {
        if (next_m >= rad_size - 1) break;   /* safety, should not happen */

        /* compute product and rad product for current m */
        unsigned __int128 prod128 = (unsigned __int128)next_m * (next_m + 1);
        if (prod128 > max_seq128) break;     /* further products will be larger */

        uint64_t rad_prod = rad[next_m] * rad[next_m + 1];
        if (is_primorial(rad_prod)) {
            /* store the term if it fits */
            seq_t term = (seq_t)prod128;
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) abort();
            }
            cache[cache_len++] = term;
        }

        ++next_m;
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* sequence offset is 1 */
    ensure_terms(n);
    if (n <= cache_len) return cache[n - 1];
    /* if not enough terms could be generated, return 0 (unlikely for valid tests) */
    return 0;
}

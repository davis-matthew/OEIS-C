/*
 * A370786  Powerful numbers with an odd number of prime factors (counted with
 *          multiplicity).
 * https://oeis.org/A370786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth powerful number with an odd total number of
 *            prime factors counted with multiplicity
 * Range:     u8 n <= 9, u16 n <= 198, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----- internal data ---------------------------------------------------- */
static uint64_t *g_cache = NULL;          /* generated sequence values */
static size_t g_cache_size = 0, g_cache_cap = 0;
static uint64_t *g_primes = NULL;          /* list of primes up to √limit */
static size_t g_primes_cnt = 0;
static bool g_initialized = false;

/* ----- helpers ---------------------------------------------------------- */

/* integer square root for 64‑bit unsigned integers */
static inline uint64_t int_sqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = n;
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid <= n / mid) /* mid*mid <= n */
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* push a value to the cache, reallocating if necessary */
static void cache_push(uint64_t val)
{
    if (g_cache_size == g_cache_cap) {
        size_t new_cap = g_cache_cap ? g_cache_cap * 2 : 256ull;
        uint64_t *tmp = (uint64_t *)realloc(g_cache, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        g_cache = tmp;
        g_cache_cap = new_cap;
    }
    g_cache[g_cache_size++] = val;
}

/* recursive DFS that generates all powerful numbers ≤ LIMIT
   with an odd total exponent sum. */
static void dfs(uint64_t *primes, size_t p_cnt,
                uint64_t cur, uint64_t sum_exp,
                uint64_t limit, size_t idx)
{
    if (idx == p_cnt) return;               /* out of primes */

    /* skip current prime */
    dfs(primes, p_cnt, cur, sum_exp, limit, idx + 1);

    uint64_t p = primes[idx];
    uint64_t pow = p * p;                   /* start with exponent 2 */
    uint64_t e   = 2ULL;

    while (pow <= limit / cur) {
        uint64_t newcur = cur * pow;
        uint64_t newsum = sum_exp + e;
        if ((newsum & 1ULL) && newcur != 1ULL)  /* odd sum and >1 */
            cache_push(newcur);

        dfs(primes, p_cnt, newcur, newsum, limit, idx + 1);

        /* next exponent */
        if (pow > limit / p) break;           /* would overflow newcur */
        pow *= p;
        ++e;
    }
}

/* create the list of primes up to maxp */
static void build_primes(uint64_t maxp, uint64_t **primes_out, size_t *cnt_out)
{
    bool *is_comp = (bool *)calloc(maxp + 1, sizeof(bool));
    if (!is_comp) exit(EXIT_FAILURE);

    size_t prime_cap = maxp / 10 + 10;
    uint64_t *primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    if (!primes) exit(EXIT_FAILURE);
    size_t cnt = 0;

    for (uint64_t i = 2; i <= maxp; ++i) {
        if (!is_comp[i]) {
            if (cnt == prime_cap) {
                prime_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                primes = tmp;
            }
            primes[cnt++] = i;
            if (i <= maxp / i) {
                for (uint64_t j = i * i; j <= maxp; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
    *primes_out = primes;
    *cnt_out    = cnt;
}

/* compare function for qsort (uint64_t) */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* initialise the cache only once */
static void init_cache(void)
{
    if (g_initialized) return;

    /* limit up to which numbers are generated (≈ 125 M for 10 000 terms) */
    const uint64_t limit = 200000000ULL;

    /* primes up to √limit */
    uint64_t maxp = int_sqrt_u64(limit);
    build_primes(maxp, &g_primes, &g_primes_cnt);

    /* allocate cache */
    g_cache_cap = 512;
    g_cache = (uint64_t *)malloc(g_cache_cap * sizeof(uint64_t));
    if (!g_cache) exit(EXIT_FAILURE);

    /* generate data */
    dfs(g_primes, g_primes_cnt, 1ULL, 0ULL, limit, 0);

    /* sort the generated numbers */
    qsort(g_cache, g_cache_size, sizeof(uint64_t), cmp_uint64);

    g_initialized = true;

    /* free primes array as it is no longer needed */
    free(g_primes);
    g_primes = NULL;
}

/* ----- public API ------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; behaviour for 0 ignored */
    init_cache();
    if (n > g_cache_size) return 0;
    return (seq_t)g_cache[n - 1];
}

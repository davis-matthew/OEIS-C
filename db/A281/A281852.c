/*
 * A281852  Expansion of Sum_{p prime, i>=1} x^(p^i) / (1 - Sum_{p prime,
 *          j>=1} x^(p^j))^2.
 * https://oeis.org/A281852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of parts in all compositions of n into prime
 *            powers (1 excluded)
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes a(n) via dynamic programming. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* external types provided by the build */
typedef uint64_t idx_t;

/* ------------------------------------------------------------ */
/* Prime list                                                  */
/* ------------------------------------------------------------ */
static uint64_t *prime_list = NULL;
static size_t   prime_cnt  = 0;
static size_t   prime_cap  = 0;
static uint64_t next_prime_candidate = 2;

static void ensure_primes(idx_t limit)
{
    for ( ; next_prime_candidate <= limit; ++next_prime_candidate )
    {
        bool is_prime = true;
        for (size_t k = 0; k < prime_cnt; ++k)
        {
            uint64_t p = prime_list[k];
            if (p > next_prime_candidate / p) break;
            if (next_prime_candidate % p == 0)
            {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
        {
            if (prime_cnt == prime_cap)
            {
                prime_cap = (prime_cap ? prime_cap << 1 : 4);
                prime_list = (uint64_t*)realloc(prime_list,
                                                prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = next_prime_candidate;
        }
    }
}

/* ------------------------------------------------------------ */
/* Prime–power list                                            */
/* ------------------------------------------------------------ */
static idx_t *prime_power   = NULL;
static size_t prime_power_cnt = 0;
static idx_t  prime_power_limit = 0;   /* largest n for which the list is valid */

static int cmp_idx(const void *a, const void *b)
{
    idx_t x = *(const idx_t*)a;
    idx_t y = *(const idx_t*)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

static void rebuild_prime_powers(idx_t limit)
{
    if (prime_power_limit >= limit) return;       /* already up to date */
    prime_power_limit = limit;

    /* collect all prime powers ≤ limit */
    size_t capacity = 32;
    size_t cnt = 0;
    idx_t *tmp = (idx_t*)malloc(capacity * sizeof(idx_t));

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_list[i];
        if (p > limit) break;
        uint64_t val = p;
        while (1)
        {
            if (cnt == capacity)
            {
                capacity <<= 1;
                tmp = (idx_t*)realloc(tmp, capacity * sizeof(idx_t));
            }
            tmp[cnt++] = (idx_t)val;
            if (val > limit / p) break;
            val *= p;
        }
    }

    /* sort the array */
    qsort(tmp, cnt, sizeof(idx_t), cmp_idx);

    /* replace old array */
    free(prime_power);
    prime_power   = tmp;
    prime_power_cnt = cnt;
}

/* ------------------------------------------------------------ */
/* Dynamic programming tables                                   */
/* ------------------------------------------------------------ */
static seq_t *f_cache     = NULL;   /* number of compositions of i */
static seq_t *g_cache     = NULL;   /* total parts in those compositions */
static idx_t  cache_size  = 0;       /* number of entries currently stored */

/* ------------------------------------------------------------ */
/* Main sequence function                                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* grow tables if necessary */
    if (n + 1 > cache_size)
    {
        idx_t new_size = n + 1;

        f_cache = (seq_t*)realloc(f_cache, new_size * sizeof(seq_t));
        g_cache = (seq_t*)realloc(g_cache, new_size * sizeof(seq_t));

        /* initialise if this is the first allocation */
        if (cache_size == 0)
        {
            f_cache[0] = 1;   /* empty composition */
            g_cache[0] = 0;
        }

        /* ensure data for prime numbers and prime powers up to n */
        ensure_primes(n);
        rebuild_prime_powers(n);

        /* compute DP values for indices not yet computed */
        idx_t start = (cache_size == 0 ? 1 : cache_size);
        for (idx_t i = start; i <= n; ++i)
        {
            seq_t sum_f = 0;
            seq_t sum_g = 0;
            /* iterate over all prime powers ≤ i */
            for (size_t k = 0; k < prime_power_cnt && prime_power[k] <= i; ++k)
            {
                idx_t d = prime_power[k];
                sum_f += f_cache[i - d];
                sum_g += g_cache[i - d] + f_cache[i - d];
            }
            f_cache[i] = sum_f;
            g_cache[i] = sum_g;
        }

        cache_size = new_size;
    }

    return g_cache[n];
}

/*
 * A309264  Numbers k such that s + t = k with 0 < s < t where t and t - s are
 *          both prime.
 * https://oeis.org/A309264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest integer k such that * there exist
 *            primes t,d with t>d and k = 2t - d
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the A309264 sequence. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIEVE_LIMIT 500000  /* large enough for n <= 10000          */

static uint8_t *prime_flag = NULL;        /* 0 = prime, 1 = composite   */
static idx_t  *prime_list  = NULL;        /* list of primes up to SIEVE_LIMIT  */
static size_t  prime_count = 0;           /* number of primes            */

static seq_t *cache_arr = NULL;           /* cached sequence values       */
static size_t  cache_cap = 0;             /* allocated capacity           */
static idx_t   cache_size = 0;            /* number of cached values      */
static idx_t   last_k_checked = 0;        /* last tested k                */

/* Initialise primes and prime flag array.  */
static void init_primes(void)
{
    if (prime_flag) return;              /* already initialised          */

    prime_flag = (uint8_t*)calloc(SIEVE_LIMIT + 1, 1);
    prime_list = (idx_t*)malloc((SIEVE_LIMIT + 1) * sizeof(idx_t));

    size_t cnt = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!prime_flag[i]) {            /* i is prime                   */
            prime_list[cnt++] = i;
            if (i * i <= SIEVE_LIMIT) {
                for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    prime_flag[j] = 1;   /* mark composite               */
            }
        }
    }
    prime_count = cnt;
    prime_list = (idx_t*)realloc(prime_list, cnt * sizeof(idx_t));
}

/* Test whether the integer k has a suitable representation. */
static bool has_representation(idx_t k)
{
    for (size_t pi = 0; pi < prime_count && prime_list[pi] < k; ++pi) {
        idx_t d = prime_list[pi];
        if ((k + d) & 1) continue;          /* t must be integer */
        idx_t t = (k + d) >> 1;
        if (t <= d) continue;               /* need s = t - d > 0 */
        if (!prime_flag[t])                  /* t must be prime   */
            return true;
    }
    return false;
}

/* Ensure the cache has room for 'needed' entries. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    cache_arr = (seq_t*)realloc(cache_arr, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Generate sequence entries until we have at least 'n' terms cached. */
static void generate_until(idx_t n)
{
    init_primes();
    while (cache_size < n) {
        ++last_k_checked;
        if (last_k_checked < 4) continue;          /* first candidate is 4 */
        if (has_representation(last_k_checked)) {
            ensure_cache_capacity(cache_size + 1);
            cache_arr[cache_size++] = (seq_t)last_k_checked;
        }
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for offset 1 */
    generate_until(n);
    return cache_arr[n - 1];
}

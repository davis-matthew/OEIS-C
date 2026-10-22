/*
 * A324758  Heinz numbers of integer partitions containing no prime indices of
 *          the parts.
 * https://oeis.org/A324758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   anti‑transitive numbers: numbers n such that for each prime
 *            divisor p of n, no prime divisor of the index of p divides n
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXN 400000   /* enough for n up to about 10000 */

static idx_t spf[MAXN + 1];
static idx_t *prime_vals = NULL;
static int *prime_index_of_value = NULL;
static size_t primes_cnt = 0;
static int initialized = 0;

static void init(void)
{
    if (initialized) return;
    memset(spf, 0, sizeof(spf));
    spf[0] = 0;
    spf[1] = 1;
    for (idx_t i = 2; i <= MAXN; ++i)
    {
        if (spf[i] == 0)
        {
            spf[i] = i;
            if (i * i <= MAXN)
            {
                for (idx_t j = i * i; j <= MAXN; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    /* build prime index mapping */
    primes_cnt = 0;
    for (idx_t i = 2; i <= MAXN; ++i)
        if (spf[i] == i) ++primes_cnt;
    prime_vals = (idx_t*)malloc((primes_cnt + 1) * sizeof(idx_t));
    prime_index_of_value = (int*)calloc((MAXN + 1), sizeof(int));
    idx_t idx = 0;
    for (idx_t i = 2; i <= MAXN; ++i)
    {
        if (spf[i] == i)
        {
            ++idx;
            prime_vals[idx] = i;
            prime_index_of_value[(size_t)i] = (int)idx;
        }
    }
    initialized = 1;
}

static bool is_antitrans(idx_t n)
{
    if (n == 1) return true;
    int primes_list[16];
    int pcnt = 0;
    idx_t m = n;
    while (m > 1)
    {
        idx_t p = spf[m];
        primes_list[pcnt++] = (int)p;
        while (m % p == 0) m /= p;
    }
    for (int i = 0; i < pcnt; ++i)
    {
        int p = primes_list[i];
        int idxp = prime_index_of_value[(size_t)p];
        idx_t t = idxp;
        while (t > 1)
        {
            idx_t q = spf[t];
            if (n % q == 0) return false;
            while (t % q == 0) t /= q;
        }
    }
    return true;
}

/* cache of computed anti‑transitive numbers */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;

static void extend_cache(idx_t wanted)
{
    while (cache_len < wanted)
    {
        while (!is_antitrans(next_candidate)) ++next_candidate;
        if (cache_len >= cache_cap)
        {
            cache_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            cache = tmp;
        }
        cache[cache_len] = (seq_t)next_candidate;
        ++cache_len;
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    init();
    if (n == 0) return (seq_t)0;   /* offset is 1, so n=0 is undefined */
    extend_cache(n);
    return cache[n - 1];
}

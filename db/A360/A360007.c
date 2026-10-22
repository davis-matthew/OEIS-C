/*
 * A360007  Positions of first appearances in the sequence giving the median
 *          of the prime indices of n (A360005(n)/2).
 * https://oeis.org/A360007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth smallest integer in the set {1} ∪ {p : p is
 *            prime} ∪ {2p : p is a prime of even index}
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* compare two seq_t values for qsort */
static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* Simple sieve of Eratosthenes.
   primes_out contains all primes ≤ limit (malloc'ed).
   cnt_out is the number of primes found. */
static void build_primes(size_t limit, uint32_t **primes_out, size_t *cnt_out)
{
    bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_prime) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    for (size_t i = 2; i <= limit; ++i) is_prime[i] = true;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* first count primes to allocate array */
    size_t primes_cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++primes_cnt;

    uint32_t *primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    if (!primes) {
        free(is_prime);
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[idx++] = (uint32_t)i;

    free(is_prime);
    *primes_out = primes;
    *cnt_out = primes_cnt;
}

/* Generate all values in the required set up to a given prime limit.
   Returns an array (malloc'ed) of seq_t values and the number of
   values via len_out. The array is sorted ascending. */
static void generate_values(size_t prime_limit, seq_t **vals_out, size_t *len_out)
{
    uint32_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes(prime_limit, &primes, &primes_cnt);

    /* worst‑case capacity: 1 + primes_cnt (primes) + primes_cnt/2 (even‑index 2p) */
    size_t temp_cap = primes_cnt * 3 + 1;
    seq_t *temp = (seq_t *)malloc(temp_cap * sizeof(seq_t));
    if (!temp) {
        free(primes);
        *vals_out = NULL;
        *len_out = 0;
        return;
    }

    size_t len = 0;
    temp[len++] = (seq_t)1;               /* the special 1 */

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        temp[len++] = (seq_t)p;           /* the prime itself */
        if ((i + 1) % 2 == 0) {           /* even index (i+1) */
            seq_t two_p = ((seq_t)p) * (seq_t)2;
            temp[len++] = two_p;
        }
    }

    /* sort the values */
    qsort(temp, len, sizeof(seq_t), cmp_seq);

    /* All values are distinct; no deduplication needed. */
    *vals_out = temp;
    *len_out = len;

    free(primes);
}

/* Global cache of precomputed values */
static seq_t *cache_vals = NULL;
static size_t cache_len = 0;
static size_t cache_limit = 0;   /* current prime limit used for generating cache */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined but safe */

    /* Ensure cache holds at least n terms */
    while (cache_len < (size_t)n) {
        size_t new_limit = (cache_limit == 0) ? 200000 : cache_limit * 2;

        /* regenerate cache */
        if (cache_vals) {
            free(cache_vals);
            cache_vals = NULL;
        }

        generate_values(new_limit, &cache_vals, &cache_len);
        cache_limit = new_limit;
    }

    return cache_vals[n - 1];
}

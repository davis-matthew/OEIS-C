/*
 * A384906  Number of maximal anti-runs of consecutive parts not increasing by
 *          1 in the prime indices of n (with multiplicity).
 * https://oeis.org/A384906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as defined in OEIS A384906:
   number of maximal anti‑runs of consecutive prime indices not
   increasing by 1 in the prime factor indices of n (with multiplicity). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PRIME 2000000          /* limit for prime generation */

static uint32_t prime_index_map[MAX_PRIME + 1];
static uint32_t primes[MAX_PRIME + 1];
static size_t prime_count = 0;
static bool primes_inited = false;

/* comparison function for qsort on idx_t */
static int cmp_idx(const void *a, const void *b)
{
    const idx_t *x = (const idx_t *)a;
    const idx_t *y = (const idx_t *)b;
    return (*x < *y) ? -1 : (*x > *y) ? 1 : 0;
}

/* sieve of Eratosthenes up to MAX_PRIME, populating primes[] and
   prime_index_map[] (1‑based index of each prime). */
static void init_primes(void)
{
    if (primes_inited) return;

    static bool is_comp[MAX_PRIME + 1];
    for (idx_t i = 2; i <= MAX_PRIME; ++i) is_comp[i] = false;
    for (idx_t i = 2; (idx_t)i * i <= MAX_PRIME; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= MAX_PRIME; j += i) {
                is_comp[j] = true;
            }
        }
    }
    idx_t idx = 1;
    for (idx_t i = 2; i <= MAX_PRIME; ++i) {
        if (!is_comp[i]) {
            prime_index_map[i] = idx;
            primes[prime_count++] = (uint32_t)i;
            ++idx;
        } else {
            prime_index_map[i] = 0;
        }
    }
    primes_inited = true;
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;

    if (!primes_inited) init_primes();

    uint64_t tmp = (uint64_t)n;
    idx_t indices[128];
    size_t cnt = 0;

    /* factorization using the precomputed primes */
    for (size_t i = 0; i < prime_count &&
                    (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp;
         ++i) {
        uint32_t p = primes[i];
        while (tmp % p == 0) {
            indices[cnt++] = (idx_t)prime_index_map[p];
            tmp /= p;
        }
    }
    if (tmp > 1) { /* remaining prime factor > sqrt(original n) */
        uint32_t p = (uint32_t)tmp;
        indices[cnt++] = (idx_t)prime_index_map[p];
    }

    /* sort the indices */
    qsort(indices, cnt, sizeof(idx_t), cmp_idx);

    /* count maximal anti‑runs (splits occur when consecutive indices differ by 1) */
    seq_t result = 0;
    if (cnt > 0) {
        result = 1;
        for (size_t i = 1; i < cnt; ++i) {
            if (indices[i] - indices[i - 1] == 1) result++;
        }
    }
    return result;
}

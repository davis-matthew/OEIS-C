/*
 * A366536  The number of unitary divisors of the cubefree numbers (A004709).
 * https://oeis.org/A366536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A034444(A004709(n)) a(n) = 2^[number of distinct prime
 *            factors of the n‑th cubefree integer]
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *caches = NULL;          /* cached a(n) values                            */
static size_t cache_cap = 0;          /* allocated cache capacity                      */
static size_t cache_len = 0;          /* number of terms currently cached              */
static uint64_t *spf = NULL;          /* smallest prime factor array                   */
static size_t spf_lim = 0;            /* current upper limit of spf array              */
static idx_t next_test = 1;           /* next integer to test for cubefreeness         */

static void ensure_spf(size_t limit)
{
    if (spf_lim >= limit) return;
    free(spf);
    spf_lim = limit;
    spf = (uint64_t*)malloc((spf_lim + 1) * sizeof(uint64_t));
    if (!spf) abort();
    for (size_t i = 2; i <= spf_lim; ++i) spf[i] = i;
    for (size_t p = 2; p * p <= spf_lim; ++p) {
        if (spf[p] == p) {
            for (size_t j = p * p; j <= spf_lim; j += p)
                if (spf[j] == j) spf[j] = p;
        }
    }
    spf[0] = 0;
    spf[1] = 1;
}

static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(caches, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    caches = tmp;
    cache_cap = new_cap;
}

static void generate_next_terms(idx_t target)
{
    /* compute an upper bound for the value we need to sieve up to */
    idx_t limit = target * 2;
    if (limit < 2) limit = 2;

    ensure_spf((size_t)limit);
    ensure_cache_capacity(target);

    while (cache_len < (size_t)target) {
        idx_t x = next_test++;
        /* factorize x */
        idx_t m = x;
        int distinct = 0;
        bool cubefree = true;
        while (m > 1) {
            idx_t p = spf[m];
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
                if (e >= 3) {        /* not cubefree */
                    cubefree = false;
                    m = 1;            /* stop remaining factorization */
                    break;
                }
            }
            if (!cubefree) break;
            ++distinct;
        }
        if (cubefree) {
            /* value = 2^distinct */
            seq_t val = (seq_t)1;
            for (int i = 0; i < distinct; ++i) val = val * (seq_t)2;
            caches[cache_len++] = val;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* offset is 1, undefined for n=0 */
    if (n - 1 < (idx_t)cache_len)             /* already computed */
        return caches[n - 1];
    generate_next_terms(n);
    return caches[n - 1];
}

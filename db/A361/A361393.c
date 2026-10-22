/*
 * A361393  Positive integers k such that 2*omega(k) > bigomega(k).
 * https://oeis.org/A361393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   2*omega(k) > bigomega(k) (positive integers k)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Linear‑time sieve that builds the smallest prime factor array
   up to at least 'bound'.  The array is grown as needed.  */
static void ensure_spf(size_t bound, uint64_t **spf_ptr, size_t *spf_sz_ptr)
{
    size_t sz = *spf_sz_ptr;
    if (bound + 1 <= sz) return;          /* already big enough */

    uint64_t *old = *spf_ptr;
    size_t new_sz = bound + 1;

    uint64_t *new_spf = (uint64_t*)realloc(old, new_sz * sizeof(uint64_t));
    if (!new_spf) exit(EXIT_FAILURE);
    *spf_ptr = new_spf;
    *spf_sz_ptr = new_sz;

    /* initialise and run linear sieve */
    for (size_t i = 0; i < new_sz; ++i) new_spf[i] = 0;

    uint64_t *primes = NULL;
    size_t primes_cap = 0, primes_cnt = 0;

    for (size_t i = 2; i < new_sz; ++i) {
        if (!new_spf[i]) {
            new_spf[i] = i;
            if (primes_cnt == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                primes = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!primes) exit(EXIT_FAILURE);
            }
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint64_t p = primes[j];
            if (p > new_spf[i] || i * p >= new_sz) break;
            new_spf[i * p] = p;
        }
    }
    free(primes);
}

/* Return true iff 2*omega(k) > bigomega(k) for the unsigned integer k. */
static bool satisfies(uint64_t k, const uint64_t *spf)
{
    if (k <= 1) return false;           /* 1 is not in the sequence */

    size_t omega = 0;          /* number of distinct prime factors */
    size_t repeats = 0;        /* total multiplicity minus distinct count */

    while (k > 1) {
        uint64_t p = spf[k];
        size_t e = 0;
        do {
            k /= p;
            ++e;
        } while (k % p == 0);
        ++omega;
        repeats += e - 1;
    }
    /* 2*omega > omega + repeats  ↔  omega > repeats  */
    return repeats < omega;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of bounds – safety */

    static seq_t *cache = NULL;       /* 0‑based cached terms */
    static size_t cache_len = 0;      /* number of cached terms */
    static uint64_t *spf = NULL;      /* smallest‑prime‑factor table */
    static size_t spf_sz = 0;         /* current size of that table */

    /* initialise SPF table for index 2 */
    if (spf_sz == 0) ensure_spf(2, &spf, &spf_sz);

    /* grow cache until we have n terms */
    while (cache_len < (size_t)n) {
        /* find the next qualifying integer */
        uint64_t current = (cache_len > 0) ? (uint64_t)cache[cache_len - 1] + 1 : 2;

        for (uint64_t m = current; ; ++m) {
            /* ensure SPF table covers m */
            if (m > spf_sz - 1) ensure_spf(m + 1, &spf, &spf_sz);

            if (satisfies(m, spf)) {
                /* enlarge cache if necessary */
                if (cache_len == 0) {
                    cache = (seq_t*)malloc(32 * sizeof(seq_t));
                } else if (cache_len % 32 == 0) {
                    cache = (seq_t*)realloc(cache,
                                            (cache_len + 32) * sizeof(seq_t));
                }
                cache[cache_len] = (seq_t)m;
                ++cache_len;
                break;
            }
        }
    }
    return cache[n - 1];
}

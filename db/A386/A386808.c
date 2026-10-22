/*
 * A386808  Numbers that have exactly one exponent in their prime
 *          factorization that is equal to 5.
 * https://oeis.org/A386808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1217, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers that have exactly one exponent equal to 5 in their prime factorization.
   The function returns the n‑th smallest such integer (n≥1). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Smallest‑prime‑factor table, lazily initialised. */
static uint64_t *spf = NULL;
static size_t     spf_lim = 0;
static const size_t INIT_SPF_LIMIT = 2000000;   /* enough for n≈10 000 */

/* Initialise or resize the SPF table up to `limit`. */
static void init_spf(size_t limit)
{
    spf = (uint64_t *)calloc(limit + 1, sizeof(uint64_t));
    if (!spf) return;
    for (size_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i) {
                    if (!spf[j]) spf[j] = i;
                }
            }
        }
    }
    spf_lim = limit;
}

/* Return true iff x has exactly one prime exponent equal to 5. */
static bool one_exp5(uint64_t x)
{
    int cnt5 = 0;
    while (x > 1) {
        uint64_t p = spf[x];
        if (!p) p = x;          /* prime larger than spf_lim */
        int exp = 0;
        while (x % p == 0) {
            x /= p;
            exp++;
        }
        if (exp == 5) {
            ++cnt5;
            if (cnt5 > 1) return false;
        }
    }
    return cnt5 == 1;
}

/* Cache of already computed terms. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static uint64_t next_test = 32;   /* smallest integer that can have an exponent 5 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, return 0 */

    if (!spf) init_spf(INIT_SPF_LIMIT);

    /* Produce terms until the n-th one is available. */
    while (cache_len < (size_t)n) {
        if (one_exp5(next_test)) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;          /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_test;   /* cast is safe (value ≤ 2·10⁶) */
        }
        ++next_test;

        /* When the test value exceeds the current sieve limit,
           extend the sieve. */
        if (next_test > spf_lim) {
            size_t new_limit = spf_lim ? spf_lim * 2 : INIT_SPF_LIMIT;
            free(spf);
            init_spf(new_limit);
        }
    }
    return cache[n - 1];
}

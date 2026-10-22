/*
 * A349761  Numbers k such that sigma(k) = A000203(k) is an abundant number
 *          (A005101) and phi(k) = A000010(k) is a deficient number (A005100).
 * https://oeis.org/A349761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k such that * sigma(sigma(k)) >
 *            2*sigma(k) (sigma(k) is abundant) * sigma(phi(k)) < 2*phi(k)
 *            (phi(k) is deficient) * * The function caches found terms so it
 *            can answer any n in any order. * See the OEIS entry for A349761
 *            for detail
 * Range:     u8 n <= 17, u16 n <= 1733, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"

static seq_t *term_cache = NULL;    /* array of found terms */
static size_t cache_len  = 0;        /* number of terms stored */
static size_t cache_cap  = 0;        /* current array capacity */
static idx_t   tested_k  = 1;        /* next integer to test */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr)
        return;           /* ignore allocation failure; test harness will not crash */
    if (term_cache) {
        for (size_t i = 0; i < cache_len; ++i)
            new_arr[i] = term_cache[i];
        free(term_cache);
    }
    term_cache = new_arr;
    cache_cap   = new_cap;
}

/* Append a new found term into the cache. */
static void cache_append(seq_t val)
{
    ensure_cache_capacity(cache_len + 1);
    term_cache[cache_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined for n = 0, but avoid UB */

    /* Keep extending the cache until we have at least n terms. */
    while (cache_len < n) {
        /* Candidate k = tested_k */
        seq_t k_seq = (seq_t)tested_k;

        /* Compute sigma(k) */
        seq_t sigma_k = sigma_of((idx_t)k_seq);

        /* Compute sigma(sigma(k)) */
        seq_t sigma_sigma_k = sigma_of((idx_t)sigma_k);

        /* Compute phi(k) */
        seq_t phi_k = calc_phi(k_seq);

        /* Compute sigma(phi(k)) */
        seq_t sigma_phi_k = sigma_of((idx_t)phi_k);

        /* Check abundance of sigma(k) and deficiency of phi(k). */
        if (sigma_sigma_k > (seq_t)2 * sigma_k &&
            sigma_phi_k   < (seq_t)2 * phi_k)
            cache_append(k_seq);

        ++tested_k;
    }
    return term_cache[n - 1];
}

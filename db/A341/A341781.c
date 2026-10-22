/*
 * A341781  Refactorable numbers (or tau numbers, A033950) k such that
 *          k/tau(k) is even, where tau(k) = A000005(k).
 * https://oeis.org/A341781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1272, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Refactorable numbers k such that k / τ(k) is an even integer (OEIS A341781).
   Brute‑force search generating terms in order.
   τ(k) is divisor_count_u64(k). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"

static seq_t *cache = NULL;       /* array of found terms */
static size_t cache_len = 0;      /* number of terms stored */
static size_t cache_cap = 0;      /* allocated capacity */
static uint64_t last_checked = 7; /* last integer tested (k=8 is first term) */

/* Ensure the cache has at least 'need' slots. */
static void cache_ensure_size(size_t need)
{
    if (need <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 8;
    while (newcap < need) newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache = tmp;
    cache_cap = newcap;
}

seq_t a(idx_t n)
{
    /* Return already computed term if present. */
    if (n <= cache_len) return cache[n - 1];

    /* Generate new terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        uint64_t k = ++last_checked;                 /* next integer to test */
        uint64_t tau = (uint64_t)divisor_count_u64(k);/* τ(k) */
        /* Check that k / τ(k) is an even integer */
        if ((k % tau == 0) && (((k / tau) & 1U) == 0)) {
            cache_ensure_size(cache_len + 1);
            cache[cache_len++] = (seq_t)k;           /* store the term */
        }
    }
    return cache[n - 1];
}

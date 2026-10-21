/*
 * A249242  Squarefree primitive abundant numbers (using the second
 *          definition: having no abundant proper divisors, cf. A091191).
 * https://oeis.org/A249242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 996, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A249242: squarefree primitive abundant numbers
 * (squarefree, abundant, and having no abundant proper divisor). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

/* Cached values of the sequence */
static seq_t *cache = NULL;     /* array of computed numbers             */
static size_t cache_cap = 0;    /* allocated capacity (number of slots) */
static size_t cache_sz  = 0;    /* number of terms actually stored      */

/* Highest integer examined so far (used to resume search) */
static idx_t last_checked = 0;

/* Ensure that 'cache' can hold at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Main sequence routine ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; behaviour for n<1 is irrelevant */
        return 0;

    /* Produce terms until we have at least 'n' of them. */
    while (cache_sz < n) {
        /* Scan integers greater than the last one examined. */
        for (idx_t x = last_checked + 1; ; ++x) {
            last_checked = x;            /* update progress marker      */

            /* Factor the candidate. */
            uint64_t pr[64];
            uint64_t ex[64];
            size_t cnt = 0;
            factor_u64((uint64_t)x, pr, ex, &cnt);

            /* Must be square‑free. */
            bool squarefree = true;
            for (size_t j = 0; j < cnt; ++j) {
                if (ex[j] > 1) {
                    squarefree = false;
                    break;
                }
            }
            if (!squarefree)
                continue;

            /* Compute σ(n) = ∏(p+1). */
            seq_t sigma = 1;
            for (size_t j = 0; j < cnt; ++j)
                sigma *= (seq_t)(pr[j] + 1);

            seq_t val = (seq_t)(uint64_t)x;

            /* Abundance: σ(n) > 2·n. */
            if (sigma <= 2 * val)
                continue;

            /* Primitivity: all proper divisors obtained by dropping
               a single prime factor must be deficient. */
            bool primitive = true;
            for (size_t j = 0; j < cnt; ++j) {
                seq_t sigma_div = sigma / (seq_t)(pr[j] + 1);
                seq_t val_div   = val   / (seq_t)pr[j];
                if (sigma_div > 2 * val_div) {
                    primitive = false;
                    break;
                }
            }
            if (!primitive)
                continue;

            /* Store the found number. */
            ensure_cache_cap(cache_sz + 1);
            cache[cache_sz++] = val;

            if (cache_sz >= n)
                break;   /* enough terms for this call */
        }
    }

    return cache[n - 1];
}

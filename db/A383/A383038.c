/*
 * A383038  Positive integers which can be expressed in the form b^r - b^s
 *          where b, r, and s are positive integers.
 * https://oeis.org/A383038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 60, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383038: Positive integers that can be written as b^r - b^s
   with b,r,s ≥ 1 and r > s.
   This implementation builds a sorted cache of all such values up to a
   growing limit.  The nth term (n ≥ 1) is returned from that cache. */

/* Standard library headers */
#include <stdlib.h>
#include <stdbool.h>

/* Comparison function for qsort (provided by the utilities library). */
#include "../utils/cmp_seq_t.h"

static seq_t *cache = NULL;     /* sorted unique terms */
static size_t cache_len = 0;    /* number of stored terms */
static size_t cache_cap = 0;    /* allocated capacity */

/* Generate terms ≤ limit and store them in the global cache (sorted, unique).
   After the generation if cache_len ≥ n, we have enough terms to answer a(n). */
static void ensure_terms(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;   /* already have the required amount */

    /* Start with a modest limit and double it until enough terms are found. */
    idx_t limit = 1000;
    while (true) {
        /* Allocate enough space for generated values (rough estimate). */
        if (cache_cap < (size_t)limit + 1024) {
            size_t new_cap = (size_t)limit + 1024;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        size_t count = 0;        /* number of values generated for this limit */

        /* Iterate over bases b ≥ 2, stopping when b^2 - b > limit. */
        for (idx_t b = 2; ; ++b) {
            seq_t bb = (seq_t)b * (seq_t)b;     /* b² */
            seq_t diff_bb = bb - (seq_t)b;      /* b² - b */
            if (diff_bb > (seq_t)limit)
                break;    /* larger b will only increase diff_bb */

            seq_t base = (seq_t)b;
            seq_t pow_s = base;                /* b^s, starting with s = 1 */

            for (idx_t s = 1; pow_s <= (seq_t)limit; ++s) {
                seq_t pow_k = base;           /* b^k, starting with k = 1 */

                while (true) {
                    seq_t diff = pow_k - (seq_t)1;   /* b^k - 1 */

                    /* If multiplication would exceed the limit, stop. */
                    if (pow_s > (seq_t)limit / diff)
                        break;

                    seq_t val = pow_s * diff;
                    if (val > (seq_t)limit)
                        break;

                    /* Store the value, expanding the array if needed. */
                    if (count == cache_cap) {
                        /* Double the capacity. */
                        size_t new_cap = cache_cap == 0 ? 1024 : cache_cap * 2;
                        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                        if (!tmp) abort();
                        cache = tmp;
                        cache_cap = new_cap;
                    }
                    cache[count++] = val;

                    /* Prepare the next k.  Stop if the next multiplication
                       would overflow seq_t or exceed limit. */
                    if (pow_k > (seq_t)limit / base)
                        break;
                    pow_k *= base;
                }

                /* Prepare the next s.  Stop if the next multiplication
                   would overflow seq_t or exceed limit. */
                if (pow_s > (seq_t)limit / base)
                    break;
                pow_s *= base;
            }
        }

        /* Sort the generated values and remove duplicates. */
        qsort(cache, count, sizeof(seq_t), cmp_seq_t);
        size_t uniq = 0;
        for (size_t i = 0; i < count; ++i) {
            if (i == 0 || cache[i] != cache[i - 1])
                cache[uniq++] = cache[i];
        }

        cache_len = uniq;
        if (cache_len >= (size_t)n)
            break;                 /* enough terms for the requested index */

        /* Need more terms – double the search limit and repeat. */
        limit *= 2;
    }
}

/* Public entry point required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined for n=0 – safe fallback */
    ensure_terms(n);
    return cache[n - 1];
}

/*
 * A323612  Numbers k such that k^2 starts with at least the same number >= 2
 *          of equal digits as does k itself.
 * https://oeis.org/A323612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 1168, u64 n <= 9807,
 *            u128 n <= 9807, big any n
 * Helpers:   digit_len, pow10, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by brute‑force search: iterate over k, test the
   leading‑digit‑run condition described in the OEIS entry. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* Helper to compute the run length of equal leading digits of x. */
static unsigned leading_run(seq_t x)
{
    seq_t temp = x;
    unsigned len = digit_len(temp);
    // numbers with fewer than two digits cannot be part of the sequence
    if (len < 2)
        return 1;

    seq_t pow = pow10(len - 1);
    seq_t first = temp / pow;
    unsigned run = 1;

    while (true) {
        /* Remove the leading digit and see if the new leading digit
           equals the original first digit. */
        temp = temp % pow;
        if (temp == 0)
            break;               /* no more digits */

        pow = pow / 10;          /* move to next lower power of 10 */
        --len;
        if (temp / pow != first)
            break;
        ++run;
    }
    return run;
}

/* Static cache for the sequence, maintained across calls. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k = 1;

/* Return the n-th term of the OEIS sequence A323612 (1‑based index). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1, allowed by requirement 4 */

    /* Compute terms up to n, appending to cache as needed. */
    while (cache_len < (size_t)n) {
        seq_t k = next_k;
        unsigned run_k = leading_run(k);
        if (run_k >= 2) {
            seq_t sq = k * k;
            unsigned run_sq = leading_run(sq);
            if (run_sq >= run_k) {
                /* Grow cache if necessary. */
                if (cache_len == cache_cap) {
                    size_t newcap = (cache_cap == 0) ? 256 : cache_cap * 2;
                    seq_t *newarr = seq_alloc_new_array(newcap);
                    for (size_t i = 0; i < cache_len; ++i)
                        newarr[i] = cache[i];
                    free(cache);
                    cache = newarr;
                    cache_cap = newcap;
                }
                cache[cache_len++] = k;
            }
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}

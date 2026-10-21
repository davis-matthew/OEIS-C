/*
 * A297271  Numbers whose base-10 digits have equal down-variation and
 *          up-variation; see Comments.
 * https://oeis.org/A297271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth positive integer whose base‑10 digits have equal
 *            down‑variation and up‑variation (OEIS A297271)
 * Range:     u8 n <= 34, u16 n <= 6561, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Internal cache of already computed terms.  The cache is expanded
   only when a larger index is requested. */
static seq_t *cached_terms = NULL;
static size_t cached_len   = 0;   /* number of terms stored */
static size_t cached_cap   = 0;   /* allocated array size */
static idx_t  next_candidate = 1; /* next integer to test */

/* Return true if the base‑10 digits of num have equal down‑variation
   and up‑variation.  This checks the definition directly. */
static bool equal_variation(idx_t num)
{
    uint8_t prev = (uint8_t)(num % 10);
    num /= 10;
    uint64_t dvar = 0;
    uint64_t uvar = 0;

    while (num > 0) {
        uint8_t cur = (uint8_t)(num % 10);
        if (cur > prev) {
            dvar += (uint64_t)(cur - prev);
        } else if (cur < prev) {
            uvar += (uint64_t)(prev - cur);
        }
        prev = cur;
        num /= 10;
    }
    return dvar == uvar;
}

/* Ensure that the cache contains at least 'target' terms.
   This routine grows the cache only when necessary. */
static void ensure_terms(idx_t target)
{
    if (target <= (idx_t)cached_len)
        return;

    /* Grow the cache capacity if needed.  Use doubling strategy. */
    if (cached_cap < (size_t)target) {
        size_t new_cap = cached_cap < 64 ? 64 : cached_cap;
        while (new_cap < (size_t)target)
            new_cap *= 2;

        seq_t *tmp = (seq_t *)realloc(cached_terms, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                 /* allocation failure – abort */
        cached_terms = tmp;
        cached_cap   = new_cap;
    }

    /* Generate terms until we reach 'target'. */
    for (size_t i = cached_len; i < (size_t)target; ++i) {
        for (;;) {
            idx_t cand = next_candidate++;
            if (equal_variation(cand)) {
                cached_terms[i] = (seq_t)cand;
                break;
            }
        }
    }
    cached_len = (size_t)target;
}

/* Public API: returns the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined; return 0 for safety */

    ensure_terms(n);
    return cached_terms[n - 1];   /* sequence is 1‑based */
}

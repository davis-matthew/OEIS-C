/*
 * A384539  Zeroless positive integers k for which for every pair of nonempty
 *          substrings that concatenate to give k one substring divides the
 *          other.
 * https://oeis.org/A384539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Brute‑force search – for each integer k ≥ 11, check all splits
 *            of its decimal representation into two non‑empty parts; if for
 *            every split one part divides the other, k is a term of A384539
 * Range:     u8 n <= 66, u16 n <= 123, u32 n <= 132, u64 n <= 132,
 *            u128 n <= 132, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Test whether k satisfies the zeroless divisibility condition. */
static bool good(seq_t k)
{
    if (k < 10) return false;          /* must have at least two digits */

    /* Count digits and reject if any digit is 0. */
    int len = 0;
    seq_t tmp = k;
    while (tmp > 0) {
        int digit = (int)(tmp % 10);
        if (digit == 0) return false;
        tmp /= 10;
        len++;
    }
    if (len <= 1) return false;        /* single‑digit numbers cannot be split */

    seq_t pow10i = 10;                 /* 10¹ */
    for (int split = 1; split < len; split++) {
        seq_t a = k % pow10i;          /* one side of the split */
        seq_t b = k / pow10i;          /* the other side          */
        if (a % b != 0 && b % a != 0)
            return false;              /* split fails */
        pow10i *= 10;                  /* advance to next power of 10 */
    }
    return true;                       /* all splits satisfied */
}

/* Cache of discovered terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t next_candidate = 11;      /* first admissible number */

/* Return the n‑th term (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, but safe */

    /* Gather terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t k = next_candidate++;
        if (!good(k)) continue;        /* skip non‑terms */

        /* Grow the cache if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 128;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;        /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = k;        /* store the term */
    }
    return cache[n - 1];                /* zero‑based array index */
}

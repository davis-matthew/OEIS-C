/*
 * A362579  Numbers k such that the decimal expansion of 1/k does not contain
 *          the digit 5.
 * https://oeis.org/A362579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 873, u32 n <= 2692, u64 n <= 2692,
 *            u128 n <= 2692, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that the decimal expansion of 1/k does not contain the digit 5. */
/* Computed by brute‑force search with cycle detection. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Check whether 1/k expansion contains no digit 5. */
static bool good_k(uint64_t k)
{
    if (k <= 1) return true; /* 1/k is integer or 1.0 */

    /* dynamic array for visited remainders, using stamp technique */
    static uint32_t *vis = NULL;
    static size_t vis_size = 0;
    static uint32_t stamp = 0;

    /* Resize array if necessary */
    if (vis_size <= k) {
        size_t new_size = vis_size ? vis_size << 1 : 64;
        while (new_size <= k) new_size <<= 1;
        uint32_t *tmp = (uint32_t *)realloc(vis, new_size * sizeof(uint32_t));
        if (!tmp) return false; /* unrecoverable, but should never happen */
        vis = tmp;
        /* initialize new part to zero (stamp 0) */
        for (size_t i = vis_size; i < new_size; ++i) vis[i] = 0;
        vis_size = new_size;
    }

    uint32_t cur_stamp = ++stamp;
    uint64_t r = 1 % k;

    while (true) {
        if (vis[r] == cur_stamp) break;   /* cycle detected */
        vis[r] = cur_stamp;
        uint64_t d = (10 * r) / k;
        if (d == 5ULL) return false;      /* found digit 5 */
        r = (10 * r) % k;
        if (r == 0) return true;          /* terminates without a 5 */
    }
    return true;                          /* cycle without 5 */
}

/* External linkage function required by the test harness */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;      /* array of found terms */
    static size_t terms_cap = 0;     /* allocated capacity */
    static idx_t terms_len = 0;      /* number of stored terms */
    static uint64_t next_k = 1;      /* next candidate to test */

    /* Allocate initial array if needed */
    if (!terms && terms_len == 0) {
        terms_cap = 64;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) return 0;         /* allocation failure -> return 0 */
    }

    /* Generate terms until we reach the requested index */
    while (terms_len < n) {
        /* Find the next valid k */
        while (!good_k(next_k)) ++next_k;

        /* Store the term */
        if (terms_len >= terms_cap) {
            terms_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failure */
            terms = tmp;
        }
        terms[terms_len] = (seq_t)next_k;
        ++terms_len;
        ++next_k;   /* advance to next candidate for future calls */
    }

    return terms[n - 1];
}

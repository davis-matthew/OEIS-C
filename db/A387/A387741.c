/*
 * A387741  Positive integers that cannot be represented as the sum plus
 *          product of a multiset of at least two integers greater than 1.
 * https://oeis.org/A387741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A387741 – Positive integers that cannot be expressed as
    (sum of a multiset ≥2 of integers>1) + (product of that multiset).
    The function enumerates all expressible numbers up to a fixed
    threshold and returns the n‑th non‑expressible value. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* limit up to which we search for expressible numbers.
   200 000 is sufficient for the first ≈10 000 terms.  */
static const idx_t LIMIT = 200000;

/* Depth‑first search that marks all numbers ≤ LIMIT that can be written
   as sum + product of a multiset of at least two integers > 1. */
static void dfs_mark(seq_t prod, seq_t sum, idx_t cnt,
                     idx_t min_factor, unsigned char *rep)
{
    const seq_t LIMIT_seq = (seq_t)LIMIT;
    /* largest factor that can still keep the product ≤ LIMIT */
    seq_t max_f_seq = LIMIT_seq / prod;
    idx_t max_f = (idx_t)max_f_seq;            /* fits in idx_t */

    for (idx_t f = min_factor; f <= max_f; ++f) {
        seq_t newp = prod * f;
        seq_t news = sum + f;
        idx_t newcnt = cnt + 1;

        if (newcnt >= 2) {
            seq_t N = newp + news;
            if (N <= LIMIT_seq)
                rep[(idx_t)N] = 1;
        }
        /* recurse with the same factor as the next minimum to keep
           multisets unordered. */
        dfs_mark(newp, news, newcnt, f, rep);
    }
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    /* static cache of the pre‑computed sequence */
    static bool initialized = false;
    static seq_t *terms = NULL;
    static idx_t terms_len = 0;

    if (!initialized) {
        /* 1. Allocate array for marking expressible numbers. */
        unsigned char *rep = (unsigned char *)calloc((size_t)LIMIT + 1, sizeof(unsigned char));
        if (!rep) abort();

        /* 2. Enumerate all expressible numbers. */
        dfs_mark((seq_t)1, (seq_t)0, (idx_t)0, (idx_t)2, rep);

        /* 3. Count and store all non‑expressible numbers. */
        idx_t cnt = 0;
        for (idx_t i = 1; i <= LIMIT; ++i)
            if (!rep[i]) ++cnt;

        terms = (seq_t *)malloc((size_t)cnt * sizeof(seq_t));
        if (!terms) abort();

        idx_t pos = 0;
        for (idx_t i = 1; i <= LIMIT; ++i)
            if (!rep[i]) terms[pos++] = (seq_t)i;

        terms_len = cnt;
        free(rep);
        initialized = true;
    }

    /* 4. Return the requested term (1‑based).  If n is out of range, return 0. */
    if (n == 0 || n > terms_len)
        return 0;
    return terms[n - 1];
}

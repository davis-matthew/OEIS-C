/*
 * A249724  Numbers k such that on row k of Pascal's triangle there is no
 *          multiple of 9 which would be less than any (potential) multiple of
 *          4 on the same row.
 * https://oeis.org/A249724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 177, u16 n <= 23590, u32 n <= 23590, u64 n <= 23590,
 *            u128 n <= 23590, big any n
 * Validated: standard, pass, 100 terms through n = 9815 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A249724
    Numbers k such that on row k of Pascal's triangle there is no multiple
    of 9 which would be less than any (potential) multiple of 4 on the same
    row.  The function `a(n)` returns the n‑th such k (1‑based index). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff row k satisfies the condition. */
static bool include_row(idx_t k)
{
    /* Rows 0 and 1 contain only 1, so they satisfy the condition. */
    if (k <= 1)
        return true;

    int v2 = 0;          /* current exponent of 2 in C(k,i) */
    int v3 = 0;          /* current exponent of 3 in C(k,i) */
    idx_t first4 = (idx_t)-1;  /* earliest i with a multiple of 4 */
    idx_t first9 = (idx_t)-1;  /* earliest i with a multiple of 9 */

    /* Iterate up to the middle; symmetry guarantees the earliest indices
       occur there. */
    for (idx_t i = 1; i <= k / 2; ++i)
    {
        /* Multiplicative step: C(k,i) = C(k,i-1) * (k-i+1) / i  */
        uint64_t num = k - i + 1;
        uint64_t den = i;

        /* Count 2‑factors. */
        v2 += __builtin_ctzll(num);
        v2 -= __builtin_ctzll(den);

        /* Count 3‑factors. */
        uint64_t t = num;
        while (t % 3ULL == 0ULL) { v3++; t /= 3ULL; }
        t = den;
        while (t % 3ULL == 0ULL) { v3--; t /= 3ULL; }

        if (v2 >= 2 && first4 == (idx_t)-1)
            first4 = i;
        if (v3 >= 2 && first9 == (idx_t)-1)
            first9 = i;

        if (first4 != (idx_t)-1 && first9 != (idx_t)-1)
            break;   /* both earliest indices found */
    }

    /* Decide based on earliest positions. */
    if (first9 == (idx_t)-1)          /* no multiple of 9 */
        return true;
    if (first4 == (idx_t)-1)          /* found a 9 but no 4 */
        return false;
    return first4 <= first9;          /* 4 should not come after 9 */
}

/*  a(n) – the n‑th term of OEIS A249724 (1‑based index). */
seq_t a(idx_t n)
{
    /* Static state for lazy generation. */
    static seq_t *terms = NULL;   /* array of previously found k's */
    static size_t terms_len = 0;  /* number of entries stored */
    static size_t terms_cap = 0;  /* allocated capacity */
    static idx_t next_k = 0;      /* next row index to examine */

    /* Ensure we have at least n terms. */
    while (terms_len < n)
    {
        /* Allocate (or grow) storage if needed before using it. */
        if (terms_len >= terms_cap)
        {
            size_t new_cap = terms_cap ? terms_cap * 2 : 1024;
            seq_t *tmp = mem_resize(seq_t, terms, new_cap);
            if (!tmp)      /* allocation failure: abort gracefully */
                return (seq_t)0;
            terms = tmp;
            terms_cap = new_cap;
        }

        if (include_row(next_k))
            terms[terms_len++] = (seq_t)next_k;

        ++next_k;
    }

    return terms[n - 1];
}

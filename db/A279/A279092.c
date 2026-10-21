/*
 * A279092  Numbers that are nontrivially palindromic in two or more
 *          consecutive integer bases.
 * https://oeis.org/A279092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A279092: numbers that are nontrivially palindromic in two or more consecutive bases.
   Computed by brute‑force search, memoised for efficiency. */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/is_palindrome.h"

/* Comparator for qsort on idx_t */
static int cmp_idx(const void *p, const void *q)
{
    const idx_t *a = (const idx_t *)p;
    const idx_t *b = (const idx_t *)q;
    return (*a > *b) - (*a < *b);
}

/* Return true if k has at least one run of two consecutive bases where its
   representation in each base is a non‑trivial palindrome. */
static bool has_consecutive(idx_t k)
{
    /* Buffer for palindrome bases; 1024 is safe for k up to at least 10^18. */
    idx_t bases[1024];
    int cnt = 0;

    /* Small bases: 2 .. floor(sqrt(k)) – full digit inspection */
    idx_t sq = (idx_t)sqrt((double)k);
    for (idx_t b = 2; b <= sq && cnt < 1024; ++b) {
        if (is_palindrome((seq_t)k, b))
            bases[cnt++] = b;
    }

    /* Large bases: only two‑digit palindromes can occur.
       For such a base b and digit a we have k = a*(b+1). */
    for (idx_t a = 1; a <= sq && cnt < 1024; ++a) {
        if (k % a != 0)
            continue;
        idx_t b = k / a - 1;
        if (b > sq && b <= k - 1 && a < b)
            bases[cnt++] = b;
    }

    if (cnt < 2)
        return false;   /* at most one palindrome base */

    /* Sort the bases to search for consecutive integers. */
    qsort(bases, cnt, sizeof(idx_t), cmp_idx);

    /* Scan for a pair of consecutive bases, skipping duplicates. */
    idx_t prev = bases[0];
    for (int i = 1; i < cnt; ++i) {
        if (bases[i] == prev)
            continue;   /* duplicate base, ignore */
        if (bases[i] == prev + 1)
            return true; /* found consecutive pair */
        prev = bases[i];
    }
    return false;
}

/* Sequence a(n) – memoised for successive calls */
seq_t a(idx_t n)
{
    static seq_t *seq     = NULL;   /* cached values */
    static size_t seq_len = 0;      /* number of cached entries */
    static size_t seq_cap = 0;      /* allocated capacity */
    static idx_t  next_k  = 1;      /* next integer to test */

    /* Grow cache until we have at least n terms */
    while (seq_len < n) {
        /* find the next integer that satisfies the property */
        while (!has_consecutive(next_k))
            ++next_k;

        /* allocate more space if needed */
        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap == 0 ? 16 : seq_cap << 1;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            seq = tmp;
            seq_cap = new_cap;
        }

        /* store the found term */
        seq[seq_len++] = next_k;
        ++next_k;           /* next candidate integer */
    }

    return seq[n - 1];
}

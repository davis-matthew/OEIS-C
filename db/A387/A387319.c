/*
 * A387319  Numbers k for which modulo 3 and modulo another multiple of 3 are
 *          coincident among the fractions (2*k mod m)/m for 1 <= m <= k.
 * https://oeis.org/A387319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 196, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A387319: numbers k for which the fractions
   (2*k mod m)/m for 1 ≤ m ≤ k contain a pair with
   m = 3 and some other multiple of 3 giving equal values. */
/* The function is implemented by brute‑force search and memoises
   already found terms for arbitrary order of calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Resize a dynamic array to hold at least `needed` elements. */
static void grow_to(size_t *cap, seq_t **arr, size_t needed)
{
    if (needed <= *cap) return;
    size_t newcap = (*cap) ? (*cap) * 2 : 8;
    while (newcap < needed) newcap *= 2;
#ifdef __cplusplus
    seq_t *tmp = reinterpret_cast<seq_t*>(realloc(*arr, newcap * sizeof(seq_t)));
#else
    seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
#endif
    if (!tmp) abort();          /* failed allocation */
    *arr = tmp;
    *cap = newcap;
}

/* Return true iff `k` satisfies the definition of the sequence. */
static bool qualifies(seq_t k)
{
    if (k < 3) return false;           /* m = 3 must exist */

    seq_t r3 = ((seq_t)2 * k) % (seq_t)3;     /* (2*k) mod 3 */

    /* Check each larger multiple of 3: m = 3*t, t ≥ 2, m ≤ k. */
    seq_t max_t = k / 3;
    for (seq_t t = 2; t <= max_t; ++t)
    {
        seq_t m   = (seq_t)3 * t;
        seq_t rem = ((seq_t)2 * k) % m;      /* (2*k) mod m */
        if (rem == r3 * t) return true; /* coincidence between m=3 and m */
    }
    return false;
}

/* Return the n‑th term of A387319 (n ≥ 1). */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;   /* found terms */
    static size_t term_cnt = 0;   /* how many collected */
    static size_t capacity   = 0; /* current allocation size */
    static idx_t next_k = 1;      /* candidate to test next */

    /* Generate terms until we have at least n. */
    while (term_cnt < n)
    {
        /* Find next candidate that qualifies. */
        while (!qualifies((seq_t)next_k))
            ++next_k;

        grow_to(&capacity, &terms, term_cnt + 1);
        terms[term_cnt++] = (seq_t)next_k;
        ++next_k;                 /* next search starts after this k */
    }

    return terms[n - 1];
}

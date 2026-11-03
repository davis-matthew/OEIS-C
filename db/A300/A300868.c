/*
 * A300868  Lexicographically earliest sequence of distinct positive terms
 *          such that, for any n > 0, n * a(n) is a Fibbinary number
 *          (A003714).
 * https://oeis.org/A300868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) is defined by a search: for each n find the smallest unused
 *            integer m such that n * m has no consecutive 1's in its binary
 *            representation. This yields a self‑inverse permutation of the
 *            natural numbers
 * Range:     u8 n <= 19, u16 n <= 610, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9408 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Ensure that the internal arrays can address idx (>=0). */
static void extend_if_needed(idx_t idx, seq_t **perm_pp, unsigned char **used_pp,
                             size_t *cap_pp)
{
    if ((size_t)idx < *cap_pp) return;

    size_t new_cap = (*cap_pp) ? *cap_pp : 4;
    while (new_cap <= (size_t)idx) new_cap <<= 1;

    *perm_pp = mem_resize(seq_t, *perm_pp, (new_cap * sizeof(**perm_pp)) / sizeof(seq_t));
    *used_pp = (unsigned char*)realloc(*used_pp, new_cap * sizeof(**used_pp));

    for (size_t i = *cap_pp; i < new_cap; ++i) {
        (*perm_pp)[i] = 0;
        (*used_pp)[i] = 0;
    }
    *cap_pp = new_cap;
}

static seq_t *perm = NULL;          /* perm[i] = a(i) */
static unsigned char *used = NULL;  /* used[i] == 1 if i already paired */
static size_t cap = 0;              /* current allocation size (>= indices + 1) */
static idx_t processed = 0;         /* largest n that has been fully paired */

/* Build the permutation up to and including up_to. */
static void ensure_up_to(idx_t up_to)
{
    if (up_to <= processed) return;

    for (idx_t i = processed + 1; i <= up_to; ++i) {
        extend_if_needed(i, &perm, &used, &cap);

        /* If i has already been paired as the partner of a smaller index,
           simply skip processing it again. */
        if (used[i]) continue;

        /* Find the smallest unused j such that i*j is a Fibbinary number. */
        for (idx_t j = 1; ; ++j) {
            extend_if_needed(j, &perm, &used, &cap);
            if (used[j]) continue;

            seq_t prod = (seq_t)i * (seq_t)j;
            if ((prod & (prod >> 1)) == 0) {
                perm[i] = j;
                perm[j] = i;
                used[i] = 1;
                used[j] = 1;
                break;
            }
        }
    }
    processed = up_to;
}

/* Required function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for out‑of‑range calls */

    extend_if_needed(n, &perm, &used, &cap);
    ensure_up_to(n);
    return perm[n];
}

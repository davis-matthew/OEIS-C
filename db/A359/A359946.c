/*
 * A359946  Lexicographically earliest sequence of distinct positive integers
 *          such that for any n > 0, n XOR a(n) is a prime number (where XOR
 *          denotes the bitwise XOR operator).
 * https://oeis.org/A359946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   ensure_perm_cap, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the lexicographically earliest self‑inverse permutation
   of the positive integers such that for all n > 0,
   n XOR a(n) is a prime.  The sequence is built greedily: for each
   unassigned n, pick the smallest unused integer m with (n XOR m)
   prime and set a(n)=m, a(m)=n. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"    /* provides bool is_prime_big(seq_t) */
#include "../utils/ensure_perm_cap.h"

static void ensure_used_cap(size_t needed, uint8_t **arr, size_t *cap)
{
    if (needed <= *cap)
        return;
    size_t newcap = *cap ? *cap * 2 : 16;
    while (newcap < needed)
        newcap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(*arr, newcap * sizeof(uint8_t));
    if (!tmp)
        abort();
    for (size_t i = *cap; i < newcap; ++i)
        tmp[i] = 0;
    *arr = tmp;
    *cap = newcap;
}

/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* static state shared between calls */
    static seq_t *perm  = NULL;          /* perm[i] = a(i)  (1‑based) */
    static size_t perm_cap = 0;
    static uint8_t *used  = NULL;        /* used[x]==1 iff x already used */
    static size_t used_cap = 0;

    /* Ensure storage for values up to n */
    if ((size_t)n + 1 > perm_cap)
        ensure_perm_cap((size_t)n + 1, &perm, &perm_cap);
    if ((size_t)n + 1 > used_cap)
        ensure_used_cap((size_t)n + 1, &used, &used_cap);

    /* Greedily assign all still unassigned positions up to n. */
    for (size_t i = 1; i <= (size_t)n; ++i) {
        if (perm[i] != 0)            /* already assigned */
            continue;

        /* Find the smallest unused integer candidate. */
        size_t cand = 1;
        for (;;) {
            if ((size_t)cand + 1 > used_cap)
                ensure_used_cap((size_t)cand + 1, &used, &used_cap);
            if (!used[cand]) {
                seq_t xored = (seq_t)i ^ (seq_t)cand;
                if (is_prime_big(xored))
                    break;
            }
            ++cand;
        }

        /* Ensure space for the partner index. */
        if ((size_t)cand + 1 > perm_cap)
            ensure_perm_cap((size_t)cand + 1, &perm, &perm_cap);
        if ((size_t)cand + 1 > used_cap)
            ensure_used_cap((size_t)cand + 1, &used, &used_cap);

        /* Assign the pair. */
        perm[i] = (seq_t)cand;
        perm[cand] = (seq_t)i;

        /* Mark both numbers as used. */
        used[i] = 1;
        used[cand] = 1;
    }

    return perm[(size_t)n];
}

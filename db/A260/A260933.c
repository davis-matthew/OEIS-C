/*
 * A260933  Lexicographically smallest permutation of the natural numbers,
 *          such that a(n)+n and a(n)+n+1 are both composite numbers.
 * https://oeis.org/A260933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* By search: lexicographically smallest involutive permutation
   a(n) where (a(n)+n) and (a(n)+n+1) are both composite. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t n); */

static idx_t *perm   = NULL;   /* perm[i] = a(i), 0 means unassigned   */
static bool  *used   = NULL;   /* used[i] = whether i has already been paired */
static size_t cap     = 0;     /* current array capacity (>= 1)       */

/* Ensure the arrays are large enough to hold indices up to `need` (inclusive). */
static void ensure_cap(idx_t need)
{
    if ((idx_t)cap >= need)
        return;
    size_t new_cap = (cap == 0) ? 64 : cap;
    while (new_cap < (size_t)need)
        new_cap <<= 1;

    idx_t *p = (idx_t *)realloc(perm, new_cap * sizeof(idx_t));
    bool  *u = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!p || !u) abort();

    perm = p;
    used = u;
    for (size_t i = cap; i < new_cap; ++i) {
        perm[i] = 0;
        used[i] = false;
    }
    cap = new_cap;
}

/* Pair i with the smallest unused j >= i such that
   i+j and i+j+1 are both composite.   */
static void pair_with(idx_t i)
{
    for (idx_t j = i; ; ++j) {
        ensure_cap(j);
        if (used[j])
            continue;
        if (!is_prime(i + j) && !is_prime(i + j + 1)) {
            perm[i] = j;
            perm[j] = i;
            used[i] = used[j] = true;
            return;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                /* offset not defined – return 0 */
        return (seq_t)0;

    ensure_cap(n);

    for (idx_t i = 1; i <= n; ++i) {
        if (perm[i] == 0)
            pair_with(i);
    }

    return (seq_t)perm[n];
}

/*
 * A297261  Numbers whose base-7 digits have greater down-variation than
 *          up-variation; see Comments.
 * https://oeis.org/A297261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th integer whose base‑7 down‑variation exceeds its
 *            up‑variation. The implementation enumerates integers, checks
 *            this property, and caches results for repeated queries
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isgood
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isgood.h"

/* Cache of computed terms. */
static seq_t *terms   = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;
static seq_t next_candidate = (seq_t)0;

/* Ensure the cache has room for at least 'need' elements. */
static void ensure_cache(size_t need)
{
    if (need <= terms_cap)
        return;
    size_t newcap = terms_cap ? terms_cap*2 : 16;
    while (newcap < need)
        newcap *= 2;
    terms = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
    terms_cap = newcap;
}

/* Return the n-th term of A297261 (offset 1). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;       /* undefined for the offset, return 0 */

    while (terms_len < (size_t)n) {
        next_candidate = next_candidate + (seq_t)1;
        if (isgood(next_candidate)) {
            ensure_cache(terms_len + 1);
            terms[terms_len++] = next_candidate;
        }
    }
    return terms[(size_t)(n-1)];
}

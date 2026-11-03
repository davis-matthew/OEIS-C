/*
 * A349057  Numbers k such that the k-th composition in standard order is not
 *          weakly alternating.
 * https://oeis.org/A349057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = n-th integer k for which the k‑th composition in standard
 * order (graded reverse‑lexicographic) is NOT weakly alternating.
 * The function returns the n-th such integer, with a(1) = 37. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

/* Return true if the composition of k (in standard order) is NOT
 * weakly alternating.  The composition is obtained by the method
 * described in the OEIS entry. */
static bool is_not_weakly_alternating(idx_t k)
{
    /* positions of 1's (starting at 1 for the least significant bit) */
    int positions[65];
    size_t pcnt = 0;
    int pos = 1;
    idx_t tmp = k;
    while (tmp) {
        if (tmp & (idx_t)1) {
            if (pcnt < sizeof positions / sizeof *positions)
                positions[pcnt++] = pos;
        }
        tmp >>= 1;
        pos++;
    }

    if (pcnt == 0) return false;   /* empty composition is weakly alternating */

    /* first differences of positions, then reverse to get the composition */
    int parts[65];
    size_t parts_len = 0;
    int prev = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        int diff = positions[i] - prev;
        parts[parts_len++] = diff;
        prev = positions[i];
    }
    /* reverse */
    for (size_t i = 0; i < parts_len / 2; ++i) {
        int t = parts[i];
        parts[i] = parts[parts_len - 1 - i];
        parts[parts_len - 1 - i] = t;
    }

    /* Test both possible starting trends: non‑decreasing then non‑increasing
     * or non‑increasing then non‑decreasing. */
    bool ok_inc = true;   /* begin with weakly increasing differences */
    bool ok_dec = true;   /* begin with weakly decreasing differences */
    for (size_t i = 1; i < parts_len; ++i) {
        int d = parts[i] - parts[i - 1];
        if ((i & 1) == 1) {          /* odd difference index (1‑based) */
            if (d < 0) ok_inc = false;
            if (d > 0) ok_dec = false;
        } else {                     /* even difference index */
            if (d > 0) ok_inc = false;
            if (d < 0) ok_dec = false;
        }
    }
    return !(ok_inc || ok_dec);
}

/* Cache of computed terms */
static seq_t *cache = NULL;          /* array of seq_t values */
static size_t cache_len = 0;         /* number of entries stored */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t processed_k = 0;        /* last k that was checked */

/* Ensure the cache has capacity for at least 'need' entries. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < need) new_cap = need;
    seq_t *newp = mem_new(seq_t, new_cap);
    if (!newp) exit(1);        /* allocation failure */
    if (cache) {
        mem_copy(newp, cache, cache_len * sizeof(seq_t));
        mem_free(cache);
    }
    cache = newp;
    cache_cap = new_cap;
}

/* Returned element type: seq_t
 * Index type: idx_t
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, return 0 */

    /* Fill the cache up to the requested index if necessary */
    while (cache_len < (size_t)n) {
        ++processed_k;
        if (is_not_weakly_alternating(processed_k)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)processed_k;
        }
    }
    return cache[n - 1];
}

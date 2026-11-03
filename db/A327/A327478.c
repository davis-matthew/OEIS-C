/*
 * A327478  Numbers whose average binary index is also a binary index.
 * https://oeis.org/A327478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the n‑th positive integer whose average binary
 * index is also a binary index.  The binary index of m is the position
 * (starting at 1 for the least significant bit) of a 1 in the reversed
 * binary expansion of m.  The average index must be integer and that
 * integer position must also contain a 1.  This property is tested
 * directly in _good_index below. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* Append val to the dynamic array *arr expanding it if needed. */
static void _append_value(seq_t val, seq_t **arr, size_t *len, size_t *cap)
{
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        seq_t *tmp = mem_new(seq_t, new_cap);
        if (*cap) {
            mem_copy(tmp, *arr, *len * sizeof(seq_t));
            mem_free(*arr);
        }
        *arr   = tmp;
        *cap   = new_cap;
    }
    (*arr)[*len] = val;
    (*len)++;
}

/* Test whether x satisfies the property */
static bool _good_index(idx_t x)
{
    idx_t pos   = 1ULL;   /* current bit position (1‑based) */
    idx_t count = 0;      /* number of set bits */
    idx_t sum   = 0;      /* sum of positions of set bits */

    idx_t temp = x;
    while (temp != 0) {
        if (temp & 1ULL) {
            count++;
            sum += pos;
        }
        temp >>= 1;
        pos++;
    }
    if (count == 0)            /* should not happen for x>=1 */
        return false;
    if ((sum % count) != 0)    /* average not integer */
        return false;
    idx_t avg = sum / count;   /* integer average (1‑based) */

    /* avg <= number of bits in x <= 64 for uint64_t, so shift safe */
    return ((x >> (avg - 1)) & 1ULL) != 0;
}

/* External function requested by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; unused entry for n=0 */
        return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t next_candidate = 1;    /* next integer to test */

    while (cache_size < n) {
        if (_good_index(next_candidate))
            _append_value((seq_t)next_candidate, &cache, &cache_size, &cache_cap);
        next_candidate++;
    }
    return cache[n - 1];
}

/*
 * A345914  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has reverse-alternating sum >= 0.
 * https://oeis.org/A345914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer k such that the reverse‑alternating sum
 *            of the k‑th composition in standard order is ≥ 0. (The value is
 *            obtained by incremental search.)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Return true if the k‑th composition (in A066099 order) has a
   non‑negative reverse‑alternating sum.  k is a non‑negative integer
   represented as an idx_t. */
static bool qualifies(idx_t k)
{
    /* Empty composition (k = 0) has sum = 0. */
    if (k == 0) return true;

    idx_t tmp = k;
    int pos = 1;          /* current bit position (1‑based, LSB→MSB) */
    int prev_pos = 0;     /* position of previous '1' bit */
    int sign = 1;         /* sign for the current difference */
    int64_t sum = 0;      /* accumulated reverse‑alternating sum */

    while (tmp) {
        if (tmp & 1ULL) {             /* bit is 1 */
            int diff = pos - prev_pos;/* run‑length for this part */
            sum += (int64_t)sign * diff;
            sign = -sign;             /* alternate sign */
            prev_pos = pos;
        }
        pos++;
        tmp >>= 1;
    }
    return sum >= 0;
}

/* Cache of qualifying k's for fast repeated queries. */
static seq_t *cache      = NULL;
static size_t cache_sz   = 0; /* number of stored terms */
static size_t cache_cap  = 0; /* current allocation size */
static idx_t next_k      = 0; /* next k to test */

/* Ensure the cache array has at least `need` capacity. */
static void ensure_cache_cap(size_t need)
{
    if (need > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 128 : cache_cap << 1;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache      = tmp;
        cache_cap  = new_cap;
    }
}

/* a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset 1 – never returned */

    /* Extend the cache until we have at least n terms. */
    while (cache_sz < (size_t)n) {
        if (qualifies(next_k)) {
            ensure_cache_cap(cache_sz + 1);
            cache[cache_sz++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}

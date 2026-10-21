/*
 * A248625  Lexicographically earliest sequence of nonnegative integers such
 *          that no triple (a(n),a(n+d),a(n+2d)) is in arithmetic progression,
 *          for any d>0, n>=0.
 * https://oeis.org/A248625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest sequence avoiding 3‑term arithmetic progressions
   (OEIS A248625). Computed greedily: for each n, pick the smallest
   non‑negative integer that does not create an arithmetic progression
   of the form (a[n‑2d], a[n‑d], a[n]) for any d > 0.  The algorithm
   builds the sequence incrementally and memoises previous results
   so that calls to a() for arbitrary n are answered in O(n²) total
   time with n ≤ 10000.  */
   
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cached sequence storage */
static seq_t *seq_cache = NULL;
static size_t seq_cap   = 0;      /* allocated capacity */
static idx_t seq_len    = 0;      /* number of filled entries */

/* Ensure the cache has capacity for at least 'needed' elements */
static void ensure_capacity(idx_t needed)
{
    if (seq_cap < needed) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 1;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – terminate program (fatal error) */
            abort();
        }
        seq_cache = tmp;
        seq_cap   = new_cap;
    }
}

/* Greedy construction of A248625 up to index 'up_to' */
static void build_up_to(idx_t up_to)
{
    if (up_to < seq_len) return;
    ensure_capacity(up_to + 1);

    for (idx_t cur = seq_len; cur <= up_to; ++cur) {
        seq_t candidate = 0;
        for (;;) {                     /* try candidates starting from 0 */
            bool ok = true;
            for (idx_t d = 1; 2 * d <= cur; ++d) {
                idx_t i = cur - 2 * d;
                idx_t j = cur - d;
                seq_t x = seq_cache[(size_t)i];
                seq_t y = seq_cache[(size_t)j];
                /* Reject if (x, y, candidate) is an arithmetic progression */
                if ((y + y) == (x + candidate)) {
                    ok = false;
                    break;
                }
            }
            if (ok) break;             /* candidate accepted */
            ++candidate;               /* try next integer */
        }
        seq_cache[(size_t)cur] = candidate;
    }
    seq_len = up_to + 1;
}

/* Public API – returns a(n) */
seq_t a(idx_t n)
{
    if (seq_len <= n) build_up_to(n);
    return seq_cache[(size_t)n];
}

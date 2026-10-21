/*
 * A248162  Partition number array: sum of part*(part - 1) for each partition
 *          of n in Abramowitz-Stegun order for n >= 1.
 * https://oeis.org/A248162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the flattened list of sums part*(part-1) over all
   partitions of k in Abramowitz‑Stegun order.  No closed‑form
   formula is known; the values are obtained by enumerating the
   partitions. */

#include <stdlib.h>

/* Cache of previously computed values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Append a new value to the cache, reallocating if necessary */
static void push_value(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!p) abort();
        cache = p;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Recursive generation of partitions in the required order
   rem          – remaining sum to partition
   max_part     – maximum allowed part (enforces non‑increasing order)
   parts_left   – how many parts remain to be chosen
   current_sum  – sum of part*(part-1) accumulated so far
*/
static void rec(idx_t rem, idx_t max_part, idx_t parts_left, seq_t current_sum)
{
    if (parts_left == 0) {
        if (rem == 0) push_value(current_sum);
        return;
    }
    if (rem == 0) return;

    /* Largest possible first part that leaves enough for the rest */
    idx_t max_first = rem - (parts_left - 1);
    if (max_first > max_part) max_first = max_part;

    /* Smallest first part (balanced partition) */
    idx_t min_first = (rem + parts_left - 1) / parts_left;   /* ceil(rem/parts_left) */

    if (min_first > max_first) return;

    for (idx_t first = max_first; ; --first) {
        seq_t add = (seq_t)first * (first - 1);
        rec(rem - first, first, parts_left - 1, current_sum + add);
        if (first == min_first) break;   /* unsigned loop guard */
    }
}

/* Generate all partitions of k and store their sums in the cache. */
static void generate_integer(idx_t k)
{
    for (idx_t m = 1; m <= k; ++m) {
        rec(k, k, m, (seq_t)0);
    }
}

/* Public function: return the n‑th term of A248162. */
seq_t a(idx_t n)
{
    static idx_t next_k = 1;    /* next integer to generate (k) */

    /* Generate terms until we have at least n values cached. */
    while (cache_len < (size_t)n) {
        generate_integer(next_k);
        ++next_k;
    }

    return cache[n - 1];
}

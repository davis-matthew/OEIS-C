/*
 * A334441  Maximum part of the n-th integer partition in Abramowitz-Stegun
 *          (sum/length/lex) order; a(0) = 0.
 * https://oeis.org/A334441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerates integer partitions in Abramowitz–Stegun order (sum/length/lex)
   and returns the maximum part of the n‑th partition.  */
#include <stddef.h>

static seq_t cache[20000];
static size_t cache_len = 0;   /* number of elements already stored */
static idx_t next_sum   = 0;   /* sum whose partitions have not yet been generated */

/* Append a value to the cache (array of seq_t). */
static inline void append_cache(seq_t v)
{
    cache[cache_len++] = v;
}

/* Recursive generation of partitions of 'sum_left' into 'parts_left' parts,
   each part not exceeding 'max_allowed' and in non‑increasing order.
   The array 'parts' holds the current partial partition.
   When a complete partition is found, its maximum part (parts[0]) is
   stored in the global cache. */
static void rec_generate(idx_t sum_left,
                         idx_t parts_left,
                         idx_t max_allowed,
                         idx_t depth,
                         seq_t *parts)
{
    if (parts_left == 0) {
        if (sum_left == 0) {
            append_cache(parts[0]);   /* maximum part is the first element */
        }
        return;
    }

    /* Smallest possible value for this part:
       ceil(sum_left / parts_left) ensures enough sum remains.       */
    idx_t low = (sum_left + parts_left - 1) / parts_left;
    /* Largest possible value:
       cannot exceed max_allowed or cause remaining sum to be too small. */
    idx_t high = max_allowed;
    idx_t max_first = sum_left - (parts_left - 1);
    if (high > max_first) high = max_first;

    if (low > high) return;

    for (idx_t p = low; p <= high; ++p) {
        parts[depth] = p;
        rec_generate(sum_left - p, parts_left - 1, p, depth + 1, parts);
    }
}

/* Generate all partitions of integer 'sum' in the required order
   and append their maximum parts to the cache. */
static void generate_sum(idx_t sum)
{
    if (sum == 0) {
        append_cache((seq_t)0);   /* empty partition */
        return;
    }

    static seq_t parts[512];   /* works for sums up to several hundred */
    for (idx_t len = 1; len <= sum; ++len) {
        rec_generate(sum, len, sum, 0, parts);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Produce terms until the requested index is available */
    while (cache_len <= n) {
        generate_sum(next_sum);
        ++next_sum;
    }
    return cache[n];
}

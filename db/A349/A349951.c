/*
 * A349951  Similar to the "Look and Say" sequence, with the added requirement
 *          that every third digit describes the location of a digit in blocks
 *          of three in the previous term.
 * https://oeis.org/A349951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   append_uint, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 * a(1) = 1.
 * For n > 1, let s = a(n‑1) written as a decimal string.
 * Divide s into successive blocks of three digits (the final block may have 1 or 2).
 * For each block with index b = 1, 2, …, process maximal runs of equal digits.
 * For a run of digit d of length c produce the decimal representation of c,
 * then d, then the decimal representation of b.
 * Concatenating these groups yields a(n).
 *
 * The function implements this rule directly and caches results to
 * support arbitrary call order.  It relies on seq_from_str to convert
 * a NUL‑terminated string into a value of type seq_t.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "../utils/seq_from_str.h"
#include "../utils/append_uint.h"

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cached_cnt = 0;          /* number of terms already cached */
    static char  *curr_str      = NULL;    /* decimal string of previous term */
    static size_t curr_len      = 0;        /* length without trailing '\0' */

    if (n < 1) return (seq_t)0;

    /* Ensure the cache can hold n terms. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise on first call. */
    if (cached_cnt == 0) {
        curr_len = 1;
        curr_str = (char *)malloc(curr_len + 1);
        if (!curr_str) return (seq_t)0;
        curr_str[0] = '1';
        curr_str[1] = '\0';
        cache[0] = seq_from_str(curr_str);
        cached_cnt = 1;
    }

    /* Generate terms up to index n. */
    for (idx_t idx = (idx_t)(cached_cnt + 1); idx <= n; ++idx) {
        /* Rough upper bound for the length of the next term. */
        size_t est_len = curr_len * 5 + 16;
        char *next_str = (char *)malloc(est_len);
        if (!next_str) return (seq_t)0;
        size_t pos = 0;

        size_t block_idx = 1;
        size_t pos_in = 0;
        while (pos_in < curr_len) {
            size_t block_start = pos_in;
            size_t block_end   = block_start + 3;
            if (block_end > curr_len) block_end = curr_len;

            size_t i = block_start;
            while (i < block_end) {
                char digit = curr_str[i];
                size_t run_len = 1;
                while ((i + run_len < block_end) && curr_str[i + run_len] == digit)
                    ++run_len;

                append_uint(next_str, &pos, run_len);
                next_str[pos++] = digit;
                append_uint(next_str, &pos, block_idx);

                i += run_len;
            }
            pos_in = block_end;
            ++block_idx;
        }
        next_str[pos] = '\0';

        cache[(size_t)(idx - 1)] = seq_from_str(next_str);
        ++cached_cnt;

        /* Prepare for the next iteration. */
        free(curr_str);
        curr_str = next_str;
        curr_len = pos;
    }

    return cache[(size_t)(n - 1)];
}

/*
 * A374757  Irregular triangle read by rows where row n lists the leaders of
 *          strictly decreasing runs in the n-th composition in standard
 *          order.
 * https://oeis.org/A374757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th output when the compositions of all
 *            non‑negative integers are listed in graded reverse‑lexicographic
 *            order (A066099). For each composition, the leaders of its
 *            strictly decreasing runs are taken. The resulting rows are then
 *            flattened into a single sequence. The function builds thi...
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Append a value to the dynamic array 'arr' with current length 'len'
   and capacity 'cap', growing it if necessary. */
static void ensure_capacity(size_t *len, size_t *cap, seq_t **arr)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Convert a non‑negative integer k (k>=1) into its composition
   in standard (graded reverse‑lexicographic) order, and generate
   the leaders of strictly decreasing runs from that composition.
   The generated leaders are appended to the cache array. */
static void process_composition(idx_t k, seq_t **cache_ptr,
                                size_t *cap_ptr, size_t *len_ptr)
{
    /* Find positions of set bits in reversed binary of k. */
    uint64_t temp = k;
    uint32_t pos[64];
    size_t pos_cnt = 0;
    uint32_t bit_index = 0;
    while (temp) {
        if (temp & 1ULL) {
            /* Positions are 1‑based: bit at index p => position p+1. */
            pos[pos_cnt++] = bit_index + 1;
        }
        temp >>= 1;
        bit_index++;
    }

    /* Build the parts by taking first differences after prepending 0. */
    seq_t parts[64];
    if (pos_cnt == 0) return; /* should not happen for k>=1 */
    parts[0] = (seq_t)pos[0];  /* first part = pos[0] - 0 */
    for (size_t i = 1; i < pos_cnt; ++i) {
        parts[i] = (seq_t)(pos[i] - pos[i-1]);
    }

    /* Reverse the parts to obtain the composition in standard order. */
    for (size_t i = 0; i < pos_cnt / 2; ++i) {
        seq_t tmp = parts[i];
        parts[i] = parts[pos_cnt - 1 - i];
        parts[pos_cnt - 1 - i] = tmp;
    }

    /* Extract leaders of strictly decreasing runs and append to cache. */
    size_t i = 0;
    while (i < pos_cnt) {
        seq_t leader = parts[i];
        size_t j = i + 1;
        while (j < pos_cnt && parts[j-1] > parts[j]) j++;
        ensure_capacity(len_ptr, cap_ptr, cache_ptr);
        (*cache_ptr)[(*len_ptr)++] = leader;
        i = j;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Static memoization state. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_k = 1;   /* start from first non‑empty composition */

    /* Generate compositions until enough terms are cached. */
    while (cache_len <= (size_t)n) {
        process_composition(next_k, &cache, &cache_cap, &cache_len);
        ++next_k;
    }
    return cache[(size_t)n];
}

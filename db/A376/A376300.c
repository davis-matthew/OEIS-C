/*
 * A376300  Numbers whose digits can be rearranged to form a substring of
 *          123456789012345678901234....
 * https://oeis.org/A376300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 952, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts, digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A376300: numbers whose digits can be rearranged to form a substring
   of 123456789012345678901234.... Computed by brute‑force search. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/digit_counts.h"
#include "../utils/digits.h"

static bool candidate_valid(idx_t cand)
{
    unsigned char cand_cnt[10] = {0};
    digit_counts((seq_t)cand, cand_cnt);

    int len = digits(cand);
    if (len <= 0) len = 1;  /* safety for 0 */

    for (int start = 0; start < 10; ++start) {
        unsigned char sub_cnt[10] = {0};
        for (int j = 0; j < len; ++j) {
            int d = (start + j) % 10;
            sub_cnt[d]++;
        }
        if (memcmp(cand_cnt, sub_cnt, sizeof(cand_cnt)) == 0)
            return true;
    }
    return false;
}

static void grow_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t newcap = (*cap ? (*cap << 1) : 16);
        while (newcap < need) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        *arr = tmp;
        *cap = newcap;
    }
}

seq_t a(idx_t n)
{
    static size_t cached_len = 0;
    static seq_t *cached_vals = NULL;
    static size_t cached_cap = 0;
    static idx_t next_candidate = 0;  /* next integer to test */

    /* Grow cache until we have at least n terms */
    while (cached_len < n) {
        if (candidate_valid(next_candidate)) {
            grow_array(&cached_vals, &cached_cap, cached_len + 1);
            cached_vals[cached_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cached_vals[n - 1];
}

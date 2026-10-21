/*
 * A261015  Irregular triangle read by rows: T(n,k) (0 <= k <= 2^n-1) = number
 *          of binary strings of length n such that the smallest number whose
 *          binary representation is not visible in the string is k.
 * https://oeis.org/A261015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the flattened sequence of the irregular triangle T(n,k)
 *            where T(n,k) counts binary strings of length n whose smallest
 *            missing binary substring is k (0 ≤ k < 2ⁿ). The row lengths are
 *            2ⁿ, so a is produced row‑by‑row
 * Range:     u8 n <= 1027, u16 n <= 9676, u32 n <= 9676, u64 n <= 9676,
 *            u128 n <= 9676, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_ROW 15                /* enough for n up to ≈ 2¹⁵‑1 ( > 10000 index ) */
#define MAX_BITS (1 << MAX_ROW)   /* 32768 */

static seq_t *seq = NULL;         /* flattened triangle */
static size_t seq_cap = 0;        /* capacity of seq */
static size_t seq_len = 0;        /* number of stored values */
static int    last_row = 0;       /* last computed row (1‑based) */

/* timestamp array to avoid clearing a large bitmap for each string */
static uint32_t seen_mark[MAX_BITS] = {0};
static uint32_t cur_mark = 1;

/* Append a row to the global sequence */
static void append_row(int r)
/* r is the row number (1‑based); row size = 1 << r */
{
    unsigned long long row_size = 1ULL << r;
    seq_t *cnt = (seq_t *)calloc(row_size, sizeof(seq_t));
    if (!cnt) return;              /* allocation failure – silently abort */

    unsigned long long all_ones = row_size - 1;
    for (unsigned long long mask = 0; mask < row_size; ++mask) {
        ++cur_mark;
        /* mark presence of pattern "0" if the string contains at least one zero */
        if (mask != all_ones) seen_mark[0] = cur_mark;

        /* generate all substrings that start with '1' */
        for (int i = 0; i < r; ++i) {
            if (((mask >> (r - 1 - i)) & 1ULL) == 0) continue;  // leading bit 0
            unsigned val = 0;
            for (int j = i; j < r; ++j) {
                val = (val << 1) | (unsigned)((mask >> (r - 1 - j)) & 1ULL);
                seen_mark[val] = cur_mark;
            }
        }

        /* find the smallest missing integer */
        unsigned long long missing = 0;
        for (missing = 0; missing < row_size; ++missing) {
            if (seen_mark[missing] != cur_mark) break;
        }
        cnt[missing] += 1;
    }

    /* ensure global array has enough space */
    if (seq_len + row_size > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 1024;
        while (seq_len + row_size > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) { free(cnt); return; }
        seq = tmp;
        seq_cap = new_cap;
    }

    /* copy new row into the global sequence */
    for (unsigned long long i = 0; i < row_size; ++i) seq[seq_len++] = cnt[i];

    free(cnt);
    last_row = r;
}

seq_t a(idx_t n)
/* External linkage as required */
{
    if (n == 0) return 0;  /* safety, off‑by‑one guard */

    /* compute rows until the global sequence covers index n */
    while (seq_len < (size_t)n) {
        append_row(last_row + 1);
    }

    return seq[(size_t)(n - 1)];
}

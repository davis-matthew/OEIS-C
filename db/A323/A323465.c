/*
 * A323465  Irregular triangle read by rows: row n lists the numbers that can
 *          be obtained from the binary expansion of n by either deleting a
 *          single 0, or inserting a single 0 after any 1, or doing nothing.
 * https://oeis.org/A323465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look, base
 * Formula:   not stated
 * Range:     u8 n <= 768, u16 n <= 8208, u32 n <= 8208, u64 n <= 8208,
 *            u128 n <= 8208, big any n
 * Validated: standard, pass, 89 terms through n = 7180 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A323465
   The nth term is the nth element of the irregular triangle
   flattened row-wise.  Row n contains all numbers obtainable
   from the binary expansion of n by either:
      • deleting a single 0,
      • inserting a single 0 after any 1,
      • doing nothing.
   Each row is sorted ascending and then concatenated. */

#include <stdint.h>
#include <stddef.h>

/* Helper: convert a binary string to seq_t */
static seq_t bin_to_num(const char *s, size_t len)
{
    seq_t val = 0;
    for (size_t i = 0; i < len; ++i) {
        val <<= 1;
        if (s[i] == '1')
            val |= 1;
    }
    return val;
}

/* Helper: convert an idx_t to binary string (most sign. bit first) */
static void idx_to_bin(idx_t x, char *buf, size_t *len)
{
    size_t pos = 0;
    while (x > 0) {
        buf[pos++] = (x & 1) ? '1' : '0';
        x >>= 1;
    }
    if (pos == 0) {          /* for x == 0, though not used here */
        buf[pos++] = '1';
    }
    /* reverse to get big-endian */
    for (size_t i = 0; i < pos / 2; ++i) {
        char tmp = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = tmp;
    }
    *len = pos;
}

/* Generate the sorted unique list of the row for given n.
   out must point to an array of at least 200 seq_t elements.
   out_cnt receives the number of unique elements. */
static void generate_row(idx_t n, seq_t *out, size_t *out_cnt)
{
    char bin[65];
    size_t bin_len;
    idx_to_bin(n, bin, &bin_len);

    seq_t tmp[200];
    size_t tmp_cnt = 0;

    /* original number */
    tmp[tmp_cnt++] = (seq_t)n;

    /* deletions of zeros */
    for (size_t i = 0; i < bin_len; ++i) {
        if (bin[i] == '0') {
            char t[65];
            size_t tl = 0;
            for (size_t j = 0; j < bin_len; ++j)
                if (j != i)
                    t[tl++] = bin[j];
            tmp[tmp_cnt++] = bin_to_num(t, tl);
        }
    }

    /* insertions of zeros after ones */
    for (size_t i = 0; i < bin_len; ++i) {
        if (bin[i] == '1') {
            char t[65];
            size_t tl = 0;
            /* copy prefix up to and including i */
            for (size_t j = 0; j <= i; ++j)
                t[tl++] = bin[j];
            /* inserted zero */
            t[tl++] = '0';
            /* copy suffix */
            for (size_t j = i + 1; j < bin_len; ++j)
                t[tl++] = bin[j];
            tmp[tmp_cnt++] = bin_to_num(t, tl);
        }
    }

    /* insertion sort */
    for (size_t i = 1; i < tmp_cnt; ++i) {
        seq_t key = tmp[i];
        size_t j = i;
        while (j > 0 && tmp[j - 1] > key) {
            tmp[j] = tmp[j - 1];
            --j;
        }
        tmp[j] = key;
    }

    /* unique */
    size_t uniq = 0;
    for (size_t i = 0; i < tmp_cnt; ++i) {
        if (uniq == 0 || tmp[i] != tmp[uniq - 1]) {
            tmp[uniq++] = tmp[i];
        }
    }

    *out_cnt = uniq;
    for (size_t i = 0; i < uniq; ++i)
        out[i] = tmp[i];
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    /* n >= 1 */
    idx_t target = n;
    idx_t cumulative = 0;

    for (idx_t row = 1; ; ++row) {
        seq_t row_vals[200];
        size_t row_cnt;
        generate_row(row, row_vals, &row_cnt);

        if (target <= cumulative + (idx_t)row_cnt) {
            return row_vals[target - cumulative - 1];
        }
        cumulative += row_cnt;
    }
}

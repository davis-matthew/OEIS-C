/*
 * A316844  Column 2 of table A316841.
 * https://oeis.org/A316844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A316841(3*n-1), i.e. the middle side j of the n-th
 *            integer triangle (i>=j>=k>=1, j+k>i), ordered by i, then j, then
 *            k
 * Range:     u8 n <= 3000, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Validated: standard, pass, 103 terms through n = 2742 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A316841_a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t pos = n - 1;
    idx_t tri_no = pos / 3;
    idx_t offset = pos % 3;

    idx_t i = 1;
    while (1) {
        idx_t j_start = (i + 1) / 2;
        idx_t cnt_tri_i = 0;
        for (idx_t j = j_start; j <= i; ++j) {
            idx_t k_start = (i > j) ? (i - j + 1) : 1;
            if (k_start <= j) {
                cnt_tri_i += (j - k_start + 1);
            }
        }
        if (tri_no < cnt_tri_i) {
            break;
        }
        tri_no -= cnt_tri_i;
        ++i;
    }

    idx_t j_start = (i + 1) / 2;
    idx_t j = j_start;
    while (1) {
        idx_t k_start = (i > j) ? (i - j + 1) : 1;
        idx_t cnt_k = 0;
        if (k_start <= j) {
            cnt_k = (j - k_start + 1);
        }
        if (tri_no < cnt_k) {
            break;
        }
        tri_no -= cnt_k;
        ++j;
    }

    idx_t k_start = (i > j) ? (i - j + 1) : 1;
    idx_t k = k_start + tri_no;

    if (offset == 0) return (seq_t)i;
    if (offset == 1) return (seq_t)j;
    return (seq_t)k;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    return A316841_a(3 * n - 1);
}

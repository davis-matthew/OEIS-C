/*
 * A378626  Table T(n, k) read by upward antidiagonals. T(n,1) = A377137(n),
 *          T(n,2) = A377137(A377137(n)), T(n,3) =
 *          A377137(A377137(A377137(n))) and so on.
 * https://oeis.org/A378626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k), the table read by upward antidiagonals, with
 *            T(r,k) = A377137 applied k times to r (A377137 iterated k times,
 *            starting from r)
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Validated: standard, pass, 134 terms through n = 8228 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static seq_t A377137_a(idx_t n)
{
    idx_t row = 1;
    seq_t acc = 0;
    while (1) {
        seq_t L = (row & 1) ? ((seq_t)row + 1) / 2 : ((seq_t)row * 3) / 2;
        if (acc + L >= (seq_t)n)
            break;
        acc += L;
        ++row;
    }
    seq_t L = (row & 1) ? ((seq_t)row + 1) / 2 : ((seq_t)row * 3) / 2;
    seq_t pos = (seq_t)n - acc - 1;
    seq_t start = acc + 1;
    seq_t end = start + L - 1;
    bool evens_first = (row % 2 == 0) || ((row % 2 == 1) && (L % 2 == 0));
    seq_t val;
    seq_t count_evens = end / 2 - (start - 1) / 2;
    if (evens_first) {
        seq_t largest_even = (end & 1) ? end - 1 : end;
        if (pos < count_evens) {
            val = largest_even - 2 * pos;
        } else {
            seq_t pos_odd = pos - count_evens;
            seq_t first_odd = (start & 1) ? start : start + 1;
            val = first_odd + 2 * pos_odd;
        }
    } else {
        seq_t count_odds = L - count_evens;
        seq_t largest_odd = (end & 1) ? end : end - 1;
        if (pos < count_odds) {
            val = largest_odd - 2 * pos;
        } else {
            seq_t pos_even = pos - count_odds;
            seq_t first_even = (start & 1) ? start + 1 : start;
            val = first_even + 2 * pos_even;
        }
    }
    return val;
}

seq_t a(idx_t n)
{
    idx_t d = 1;
    while ((d * (d + 1)) / 2 < n)
        ++d;
    idx_t i = n - (d * (d - 1)) / 2;   /* 1..d */
    idx_t row = d + 1 - i;
    idx_t k = i;
    seq_t v = (seq_t)row;
    for (idx_t j = 0; j < k; ++j)
        v = A377137_a((idx_t)(long long)v);
    return v;
}

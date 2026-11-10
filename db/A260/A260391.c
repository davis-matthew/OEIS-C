/*
 * A260391  Positions of 0 in the infinite palindromic word at A260390.
 * https://oeis.org/A260391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in the infinite palindromic word
 *            A260390 (found by scanning A260390)
 * Range:     u8 n <= 112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9899 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A260390_a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    while (n > 2) {
        idx_t len = 2;
        idx_t k = 1;
        while (len < n) {
            len = 2 * len + 1;
            k++;
        }
        idx_t prev_len = (len - 1) / 2;
        idx_t midpos = prev_len + 1;
        if (n == midpos) {
            return A260390_a(k - 1);
        }
        n = 2 * prev_len + 2 - n;
    }
    return (n == 1) ? (seq_t)1 : (seq_t)0;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 0;
    while (count < n) {
        m++;
        if (A260390_a(m) == 0) count++;
    }
    return (seq_t)m;
}

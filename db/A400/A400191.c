/*
 * A400191  First differences of A365872, divided by 2.
 * https://oeis.org/A400191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (A365872(n+1) - A365872(n))/2, with A365872(1)=1 and
 *            A365872(k) the least m > A365872(k-1) such that m - A365872(k-1)
 *            = 2*(10*lastdigit(A365872(k-1)) + firstdigit(m)); computed by
 *            direct search
 * Range:     u8 n <= 2, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A400191_first_digit(seq_t x) {
    while (x >= (seq_t)10) {
        x /= (seq_t)10;
    }
    return x;
}

seq_t a(idx_t n) {
    if (n < (idx_t)1) return (seq_t)0;

    seq_t prev = (seq_t)1;
    seq_t diff = (seq_t)0;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        seq_t last = prev % (seq_t)10;
        seq_t best = (seq_t)0;
        for (int d = 1; d <= 9; ++d) {
            seq_t candidate = prev + (seq_t)2 * (((seq_t)10 * last) + (seq_t)d);
            if (A400191_first_digit(candidate) != (seq_t)d) continue;
            if (best == (seq_t)0 || candidate < best) best = candidate;
        }
        if (best == (seq_t)0) return (seq_t)0;
        diff = (best - prev) / (seq_t)2;
        prev = best;
    }
    return diff;
}

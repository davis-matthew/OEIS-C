/*
 * A276335  Discard the most significant digit when n is expressed in greedy
 *          A001563-base (A276326), then convert back to decimal: a(n) = n -
 *          A276334(n).
 * https://oeis.org/A276335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n - A276334(n); drop the leading greedy digit in base
 *            A001563 (weights k*k!), so a(n) = n mod w, where w is the
 *            largest k*k! <= n (a(0) = 0)
 * Range:     u8 n <= 425, u16 n <= 4320, u32 n <= 4320, u64 n <= 4320,
 *            u128 n <= 4320, big any n
 * Validated: standard, pass, 113 terms through n = 3891 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t A001563_a(idx_t n) {
    seq_t result = 1;
    for (seq_t i = 2; i <= (seq_t)n; i++) {
        result *= i;
    }
    result *= (seq_t)n;
    return result;
}

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    if (n == 0) return 0;
    seq_t f = 1;          /* k! */
    seq_t w = 1;          /* A001563(k), k = 1 */
    idx_t k = 1;
    while (k < 60) {
        seq_t k1 = (seq_t)(k + 1);
        seq_t nf = f * k1;
        if (nf / k1 != f) break;
        seq_t nw = nf * k1;
        if (nw / k1 != nf) break;
        if (nw > N) break;
        f = nf;
        w = nw;
        k++;
    }
    (void)A001563_a;
    return N % w;
}

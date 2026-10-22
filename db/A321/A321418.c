/*
 * A321418  Number of n element multisets of the 15th roots of unity with zero
 *          sum.
 * https://oeis.org/A321418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in (1 - x^15)/((1 - x^3)^5 * (1 -
 *            x^5)^3) Equivalently, let A(n) = Σ_{k,l≥0, 3k+5l=n} C(k+4,4) *
 *            C(l+2,2) then a(n) = A(n) - A(n-15) (with A(m)=0 for m<0)
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 421, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  // for size_t, if needed

static seq_t binom4(idx_t k) {
    seq_t a = (seq_t)(k + 1);
    a *= (k + 2);
    a *= (k + 3);
    a *= (k + 4);
    a /= (seq_t)24;  /* 4! = 24 */
    return a;
}

static seq_t binom2(idx_t l) {
    seq_t a = (seq_t)(l + 1);
    a *= (l + 2);
    a /= (seq_t)2;   /* 2! = 2 */
    return a;
}

static seq_t A(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 0; 3 * k <= n; ++k) {
        idx_t rest = n - 3 * k;
        if (rest % 5 != 0) continue;
        idx_t l = rest / 5;
        seq_t term = binom4(k);
        term *= binom2(l);
        sum += term;
    }
    return sum;
}

seq_t a(idx_t n) {
    if (n < 15) return A(n);
    return A(n) - A(n - 15);
}

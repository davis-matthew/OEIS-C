/*
 * A365690  G.f. A(x) satisfies A(x) = 1 + x^2*A(x)^4 / (1 - x*A(x)).
 * https://oeis.org/A365690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} C(n-k-1, n-2k) * C(n+2k+1, k) / (n+2k+1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 72,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper functions binomial and gcd from the utils library */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

/* Helper: computes (a * b) / d  with exact division, reducing by gcd to avoid overflow */
static inline seq_t mul_div(seq_t a, seq_t b, seq_t d) {
    seq_t g1 = gcd(a, d);
    a   /= g1;
    d   /= g1;
    seq_t g2 = gcd(b, d);
    b   /= g2;
    d   /= g2;
    /* d should now be 1; final multiplication fits in seq_t */
    return a * b;
}

seq_t a(idx_t n) {
    /* Handle the trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    seq_t sum = 0;
    idx_t half = n / 2;          /* maximum k */
    for (idx_t k = 0; k <= half; ++k) {
        idx_t a1 = n - k - 1;    /* top of first binomial */
        idx_t b1 = n - 2 * k;    /* bottom of first binomial */
        idx_t a2 = n + 2 * k + 1;/* top of second binomial */
        idx_t b2 = k;            /* bottom of second binomial */

        /* Compute binomials */
        seq_t bin1 = binomial(a1, b1); /* C(a1, b1) */
        seq_t bin2 = binomial(a2, b2); /* C(a2, b2) */

        /* Denominator (n + 2k + 1) */
        seq_t denom = (seq_t)a2;

        /* Exact term = bin1 * bin2 / denom */
        seq_t term = mul_div(bin1, bin2, denom);

        sum += term;
    }
    return sum;
}

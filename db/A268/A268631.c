/*
 * A268631  Number of ordered pairs (a,b) of positive integers less than n
 *          with the property that n divides ab.
 * https://oeis.org/A268631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{a=1}^{n-1} ⌊ (n-1)·gcd(a,n) / n ⌋
 * Range:     u8 n <= 23, u16 n <= 947, u32 n <= 9300, u64 n <= 9300,
 *            u128 n <= 9300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts, for each a < n, the number of b < n with n | a·b. */
#include <stdint.h>

/* Compute gcd for unsigned 64‑bit integers. */
static inline idx_t gcd_u64(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Sequence value for index n. */
seq_t a(idx_t n) {
    if (n <= 1)
        return 0;               /* a(1) = 0, a(0) is irrelevant */

    seq_t sum = 0;
    idx_t n_minus_1 = n - 1;

    for (idx_t a_val = 1; a_val < n; ++a_val) {
        idx_t g = gcd_u64(a_val, n);
        seq_t prod = ((seq_t)n_minus_1) * ((seq_t)g);
        sum += prod / ((seq_t)n);
    }
    return sum;
}

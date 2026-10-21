/*
 * A271558  a(n) = G_n(11), where G is the Goodstein function defined in
 *          A266201.
 * https://oeis.org/A271558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = G_n(11), where G is the Goodstein function defined in
 *            A266201. The function computes the n-th Goodstein value starting
 *            from 11 by iterating the hereditary base increment operation
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* Recursive helper that computes B_b(m),
   i.e. the result of replacing every base b by (b+1) in the hereditary
   base-b representation of m. */
static seq_t B_b(seq_t b, seq_t m) {
    /* Base case: if m < b, no base b occurs in the representation. */
    if (m < b) return m;

    /* Find the largest power of b that does not exceed m. */
    seq_t pow = (seq_t)1;
    seq_t e   = (seq_t)0;
    seq_t next;
    while (1) {
        next = pow * b;
        if (next <= m) {
            pow = next;
            e   = e + (seq_t)1;
        } else {
            break;
        }
    }

    /* Coefficient for this largest term. */
    seq_t c = m / pow;          /* c < b */
    seq_t r = m % pow;          /* remainder < pow */

    /* Recursive evaluation of the exponent and the remainder. */
    seq_t exp_val = B_b(b, e);
    seq_t pow_new = big_pow(b + (seq_t)1, exp_val);  /* (b+1)^(exp_val) */
    seq_t term1   = c * pow_new;
    seq_t term2   = B_b(b, r);

    return term1 + term2;
}

/* Public function that returns the n-th term of the sequence. */
seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)11;   /* a(0) = 11 */

    seq_t m = (seq_t)11;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        /* Old base = i+1. */
        m = B_b((seq_t)(i + (idx_t)1), m) - (seq_t)1;
    }
    return m;
}

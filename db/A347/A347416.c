/*
 * A347416  a(n) = Sum_{k=1..n} floor((n/k)^n).
 * https://oeis.org/A347416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} ⌊(n/k)^n⌋
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided arbitrary‑precision helper big_pow, which computes
   base^exp as a seq_t.  Only integer arithmetic is performed. */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Handle the trivial case (not needed for the official offset, but safe). */
    if (n == 0) return 0;

    /* Pre‑compute n^n once. */
    seq_t n_pow = big_pow((seq_t)n, (seq_t)n);

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t k_pow   = big_pow((seq_t)k, (seq_t)n); /* k^n */
        seq_t term    = n_pow / k_pow;               /* floor((n/k)^n) */
        sum         += term;
    }
    return sum;
}

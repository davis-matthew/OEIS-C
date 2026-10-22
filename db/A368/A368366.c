/*
 * A368366  AGM transform of positive integers (see Comments for definition).
 * https://oeis.org/A368366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n(n+1)/2)^n – n^n * n! (for n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   factorial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"
#include "../utils/factorial.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset 1; a(0) unused */
    seq_t n_seq = (seq_t)n;
    seq_t mid = n_seq * (n_seq + 1) / 2;   /* (n(n+1)/2) */
    seq_t term1 = seq_pow(mid, n);          /* mid^n */
    seq_t term2 = seq_pow(n_seq, n) * factorial(n); /* n^n * n! */
    return term1 - term2;                  /* a(n) */
}

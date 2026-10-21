/*
 * A275551  Number of classes of endofunctions of [n] under vertical
 *          translation mod n and reversal.
 * https://oeis.org/A275551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 0 * (n^(n-1) + n^((n-1)/2))/2 if n is odd *
 *            n^(n-1)/2 + n^(n/2 - 1) if n is even
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t n_seq = (seq_t)n;
    if (n & 1ULL) { /* odd n */
        seq_t term1 = big_pow(n_seq, (seq_t)(n - 1));          /* n^(n-1) */
        seq_t term2 = big_pow(n_seq, (seq_t)((n - 1) / 2));    /* n^((n-1)/2) */
        return (term1 + term2) / (seq_t)2;
    } else { /* even n */
        seq_t term1 = big_pow(n_seq, (seq_t)(n - 1));          /* n^(n-1) */
        term1 = term1 / (seq_t)2;                             /* n^(n-1)/2 */
        seq_t term2 = big_pow(n_seq, (seq_t)(n / 2 - 1));      /* n^(n/2-1) */
        return term1 + term2;
    }
}

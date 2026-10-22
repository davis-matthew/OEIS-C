/*
 * A385485  a(n) is the least number k such that k*n is not a binary Niven
 *          number (A049445).
 * https://oeis.org/A385485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = least k such that k*n is not a binary Niven number
 *            (A049445)
 * Range:     u8 n <= 11, u16 n <= 4079, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: popcount for seq_t */
static seq_t popcount_seq(seq_t x) {
    seq_t cnt = 0;
    while (x) {
        cnt += (seq_t)(x & 1);
        x >>= 1;
    }
    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* n is 1-based index; we use 1 as the first term */
    idx_t k = 1;
    while (1) {
        /* product = k * n */
        seq_t prod = ((seq_t)k) * ((seq_t)n);
        seq_t pc = popcount_seq(prod);
        /* If pc == 0 theoretically impossible (only 0). Guard anyway. */
        if (pc == 0) {
            return k;
        }
        if (prod % pc != 0) {
            return k;
        }
        ++k;
    }
}

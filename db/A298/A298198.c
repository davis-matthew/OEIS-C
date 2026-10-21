/*
 * A298198  Number of Eulerian cycles in the graph Cartesian product of C_n
 *          and a double edge.
 * https://oeis.org/A298198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + 3)*6^(n-1) - 2^(n-1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small power function using exponentiation by squaring.
   It works for any seq_t type that supports *, +=, &, >>, and 1. */
static seq_t seq_pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1U)
            result *= base;
        if (exp > 1U)
            base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0)          /* offset 1: a(1) is first term; below offset undefined */
        return 0;
    idx_t exp = n - 1;   /* exponent for powers */
    seq_t pow6 = seq_pow_seq((seq_t)6, exp);
    seq_t pow2 = seq_pow_seq((seq_t)2, exp);
    seq_t coeff = (seq_t)(2 * n + 3);  /* 2*n+3 fits in idx_t, promote to seq_t */
    return coeff * pow6 - pow2;
}

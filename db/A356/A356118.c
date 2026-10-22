/*
 * A356118  Row sums of A174159.
 * https://oeis.org/A356118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n! – Catalan(n) (Catalan(n) = binomial(2n,n)/(n+1))
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* For n = 0, the loop is skipped and a = 2*1 – 1 = 1 */
    seq_t fact = 1;   /* factorial, fact = i! after iteration i */
    seq_t cat  = 1;   /* Catalan, cat  = C_i after iteration i */

    for (idx_t i = 1; i <= n; ++i) {
        fact = fact * (seq_t)i;                      /* fact = i!   */
        /* cat_{i} = cat_{i-1} * (4*i - 2) / (i + 1)  */
        cat = (cat * (seq_t)(4 * i - 2)) / (seq_t)(i + 1);
    }

    /* a(n) = 2*n! - C_n */
    return ((seq_t)2 * fact) - cat;
}

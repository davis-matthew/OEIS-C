/*
 * A349963  a(n) = Sum_{k=0..n} (2*k)^n.
 * https://oeis.org/A349963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (2*k)^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* fast exponentiation for arbitrary‑precision seq_t */
static seq_t pow_big(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t total = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t base = ((seq_t)k) * (seq_t)2U;
        seq_t term = pow_big(base, n);
        total += term;
    }
    return total;
}

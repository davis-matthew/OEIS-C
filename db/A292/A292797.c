/*
 * A292797  2-Hankel transform of ((2*n - 1)!!, 2^n * n).
 * https://oeis.org/A292797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Prod_{k=0..n} ((k+2)*(k+1)^2)^(floor((n-k)/2))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function computes the product directly using binary exponentiation. */
#include <stddef.h>
#include <stdint.h>

/* Helper: compute base^exp for seq_t values, exponent given as idx_t.
   Multiplication is performed modulo the width of seq_t (automatic wrap). */
static seq_t pow_seq_idx(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1U) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t exp = (n - k) / 2;
        if (exp == 0) continue;               /* base^0 == 1, skip multiplication */
        seq_t base = (seq_t)(k + 2) *
                     ((seq_t)(k + 1) * (seq_t)(k + 1));
        seq_t power = pow_seq_idx(base, exp);
        result *= power;
    }
    return result;
}

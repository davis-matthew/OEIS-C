/*
 * A323575  a(n) = Product_{k=1..n} (k^k + (n-k)^k).
 * https://oeis.org/A323575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (k^k + (n-k)^k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Generic integer exponentiation: base^exp where exp is non‑negative */
static seq_t
pow_uint(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1u) result *= base;
        if (exp > 1u) base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t result = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t k_pow   = pow_uint((seq_t)k, k);
        idx_t nk      = n - k;
        seq_t nk_pow  = pow_uint((seq_t)nk, k);
        seq_t term    = k_pow + nk_pow;
        result *= term;
    }
    return result;
}

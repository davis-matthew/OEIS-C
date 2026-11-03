/*
 * A258080  a(n) = ((n concatenated with n+1) + (n+1 concatenated with n)) mod
 *          n*(n+1).
 * https://oeis.org/A258080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ((n concatenated with n+1) + (n+1 concatenated with n))
 *            mod n*(n+1)
 * Range:     u8 n <= 8, u16 n <= 105, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 70 terms through n = 1978 (2026-11-01)
 * Helpers:   num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/num_digits0.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Defensive check – the sequence is defined for n >= 1 */
    if (n == 0) return (seq_t)0;

    idx_t np1 = n + 1;
    idx_t digits_n   = num_digits0(n);
    idx_t digits_np1 = num_digits0(np1);

    seq_t pow_n   = pow10(digits_n);
    seq_t pow_np1 = pow10(digits_np1);

    seq_t x = ((seq_t)n)   * pow_np1 + (seq_t)np1;
    seq_t y = ((seq_t)np1) * pow_n   + (seq_t)n;
    seq_t sum = x + y;

    seq_t denom = ((seq_t)n) * ((seq_t)n + (seq_t)1);
    return sum % denom;
}

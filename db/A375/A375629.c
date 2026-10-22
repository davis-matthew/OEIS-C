/*
 * A375629  Expansion of e.g.f. exp(2*x) / (1 - x^2 * exp(x)).
 * https://oeis.org/A375629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (k+2)^(n-2*k)/(n-2*k)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* fast exponentiation for seq_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t r = 1; /* r = n!/(n-2k)! for current k */

    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t exp_int = n - 2 * k;                     /* exponent (n-2k) */
        seq_t pow_val = (exp_int == 0) ? 1 : pow_seq((seq_t)(k + 2), exp_int);
        seq_t term = r * pow_val;
        total += term;

        /* update r for next k, if any */
        if (k < n / 2) {
            seq_t a_val = (seq_t)(n - 2 * k);
            seq_t b_val = (seq_t)(n - 2 * k - 1);
            r *= a_val;
            r *= b_val;
        }
    }

    return total;
}

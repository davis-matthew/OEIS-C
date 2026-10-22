/*
 * A355575  a(n) = n! * Sum_{k=0..floor(n/3)} k^(n - 3*k)/k!.
 * https://oeis.org/A355575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} k^(n-3k)/k!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast binary exponentiation for seq_t (may be an int or big‑int class) */
static seq_t power_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    seq_t result = 0;
    seq_t ratio = fact;          /* ratio = n! / k!   (updated each loop) */
    idx_t maxk = n / 3;

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t exp = n - 3 * k;   /* exponent in k^(n-3k) */

        seq_t k_pow;
        if (k == 0) {
            /* By convention 0^0 = 1, 0^m (m>0) = 0 */
            k_pow = (exp == 0) ? (seq_t)1 : (seq_t)0;
        } else {
            k_pow = power_seq((seq_t)k, exp);
        }

        seq_t term = ratio * k_pow;
        result += term;

        if (k < maxk)
            ratio /= (seq_t)(k + 1);   /* exact division: (n!)/((k+1)!) */
    }

    return result;
}

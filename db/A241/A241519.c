/*
 * A241519  Denominators of b(n) = b(n-1)/2 + 1/(2*n), b(0)=0.
 * https://oeis.org/A241519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator of b(n) where b(0) = 0 and b(n) = b(n-1)/2 +
 *            1/(2*n) (n >= 1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, if needed */

/* Euclidean algorithm for seq_t (works for integral or arbitrary‑precision types). */
static seq_t _gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Compute a(n) = denominator of b(n). */
seq_t a(idx_t n)
{
    /* Base case: b(0) = 0/1, so denominator is 1. */
    if (n == 0u)
        return 1;

    seq_t num = 0;   /* numerator of b(k-1) */
    seq_t den = 1;   /* denominator of b(k-1) */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t k_seq = (seq_t)k;        /* Convert k to seq_t once */

        /* b(k) = (num/k_seq + den)/(2*k_seq)  => num' = num*k + den,
           den' = 2*den*k. */
        seq_t new_num = num * k_seq + den;
        seq_t new_den = den * k_seq * 2;

        /* Reduce the fraction */
        seq_t g = _gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    return den;   /* denominator of b(n) */
}

/*
 * A322090  One of the two successive approximations up to 13^n for the
 *          13-adic integer sqrt(3). Here the 7 (mod 13) case (except for n =
 *          0).
 * https://oeis.org/A322090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = unique integer x with 0 <= x < 13^n,
 *             x^2 ≡ -3 (mod 13^n) and x ≡ 7 (mod 13).
 * 
 * The function computes the sequence by successive Hensel lifting
 * from modulus 13 to 13^n.  It uses only integer arithmetic
 * available for `seq_t`.
 * 
 * For 0 ≤ n < 1 the value is 0 (by definition of the sequence).
 * For n ≥ 1 the value is produced by lifting the root modulo
 * 13^k for k = 1 … n.
 */

#include <stdint.h>   /* for idx_t */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* current power of 13 (13^k) */
    seq_t pow13 = (seq_t)13;              /* k = 1 */
    /* root modulo current power (always ≡ 7 mod 13) */
    seq_t x = (seq_t)7;                   /* a(1) */
    /* m such that x^2 + 3 = m * pow13  (initially 13^1) */
    seq_t m = (x * x + (seq_t)3) / pow13; /* m = 4 for k = 1 */

    /* lift from k = 1 up to k = n-1 */
    for (idx_t k = 1; k < n; ++k) {
        /* current residue of m modulo 13 */
        seq_t m_mod13 = m % (seq_t)13;
        /* t = -m mod 13  (chosen 0..12) */
        seq_t t = (m_mod13 == (seq_t)0) ? (seq_t)0 : ((seq_t)13 - m_mod13);

        /* keep old x and pow13 to use in updates */
        seq_t old_x  = x;
        seq_t pow13_prev = pow13;           /* 13^k before update */
        seq_t pow13_div13 = pow13_prev / (seq_t)13;  /* 13^(k-1) */

        /* update root: x_{k+1} = x_k + t * 13^k */
        x = old_x + pow13_prev * t;

        /* update m:
         *   m_{k+1} = (m_k + 2*t*old_x) / 13
         *             + t^2 * 13^(k-1)
         */
        seq_t temp1   = ((seq_t)2 * t) * old_x;   /* 2*t*old_x  (big int) */
        seq_t sum1    = m + temp1;                /* m + 2*t*old_x */
        seq_t m_div   = sum1 / (seq_t)13;         /* exact division */
        seq_t t_sq    = t * t;                    /* t^2 (small) */
        seq_t t_sq_mul = t_sq * pow13_div13;      /* t^2 * 13^(k-1) */
        m = m_div + t_sq_mul;

        /* advance power of 13 for next iteration */
        pow13 = pow13_prev * (seq_t)13;           /* 13^(k+1) */
    }

    return x;
}

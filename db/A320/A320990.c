/*
 * A320990  Fibonacci rabbit sequence number n coded in base eight.
 * https://oeis.org/A320990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1. For n≥2
 * a(n) = a(n-1) * 8^{F_{n-1}} + a(n-2)
 * where F_k are Fibonacci numbers with F_1=1, F_2=1.
 * This generates the Fibonacci‑rabbit sequence coded in base 8. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial cases directly. */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* Iterate the recurrence.  We keep only three values:
     *   a_im2 : a(i-2), a_im1 : a(i-1), and powers of 8
     *   pow_prev2 : 8^{F_{i-2}}, pow_prev1 : 8^{F_{i-1}}
     */
    seq_t a_im2 = (seq_t)0;      /* a(0) */
    seq_t a_im1 = (seq_t)1;      /* a(1) */

    seq_t pow_prev2 = (seq_t)8;  /* 8^{F_1} = 8 */
    seq_t pow_prev1 = (seq_t)8;  /* 8^{F_2} = 8 */

    for (idx_t i = 2; i <= n; ++i) {
        /* Compute a(i) = a(i-1)*8^{F_{i-1}} + a(i-2) */
        seq_t a_new = a_im1 * pow_prev2 + a_im2;

        /* Prepare values for the next iteration if needed. */
        if (i < n) {
            seq_t next_pow_prev1 = pow_prev1 * pow_prev2;  /* 8^{F_i} = 8^{F_{i-1}} * 8^{F_{i-2}} */
            a_im2 = a_im1;
            a_im1 = a_new;
            pow_prev2 = pow_prev1;
            pow_prev1 = next_pow_prev1;
        } else {   /* last step, nothing more to compute */
            a_im2 = a_im1;
            a_im1 = a_new;
        }
    }
    return a_im1;
}

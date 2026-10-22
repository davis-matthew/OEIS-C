/*
 * A318438  For any n >= 0 with binary expansion Sum_{k=0..w} b_k * 2^k, let
 *          h(n) = Sum_{k=0..w} b_k * (i-1)^k (where i denotes the imaginary
 *          unit); a(n) is the real part of h(n).
 * https://oeis.org/A318438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, base
 * Formula:   a(n) = sum_{k where bit k of n is 1} Re((i-1)^k)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Real part of (i-1)^k follows a recurrence:  r_{k+1} = -r_k - s_k,
   s_{k+1} = r_k - s_k  with r_0 = 1, s_0 = 0.  The array `real`
   holds Re((i-1)^k) for k = 0..63.  Then a(n) is the sum of the real
   parts of the powers whose bits are set in n.  The code works for
   any seq_t and idx_t satisfying the problem statement. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 64-bit indices: shift left by up to 63 bits is safe with idx_t.   */
    static seq_t real[64] = {0};
    static bool initialized = false;

    if (!initialized) {
        /* Precompute the real parts of (i-1)^k for k = 0..63.       */
        seq_t r = 1;   /* real part of (i-1)^0   */
        seq_t s = 0;   /* imag part of (i-1)^0   */
        real[0] = r;
        for (unsigned k = 1; k < 64; ++k) {
            seq_t r_next = -r - s;   /* new real = -r - s          */
            seq_t s_next =  r - s;   /* new imag =  r - s          */
            r = r_next;
            s = s_next;
            real[k] = r;
        }
        initialized = true;
    }

    seq_t sum = 0;
    idx_t m = n;
    /* For each bit position, if the bit is set, add the corresponding
       precomputed real part.  Looping over all 64 bits is trivial in
       performance.                                                    */
    for (unsigned k = 0; k < 64; ++k) {
        if (m & ((idx_t)1 << k))
            sum += real[k];
    }

    return sum;
}

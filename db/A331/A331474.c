/*
 * A331474  Hankel transform of A331473.
 * https://oeis.org/A331474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 20, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A331474
   Recurrence: for n ≥ 8,
     a(n) = -14*a(n-2) - 51*a(n-4) - 14*a(n-6) - a(n-8)
   Initial values (indices 0–7):
     a(0)=1, a(1)=3, a(2)=2, a(3)=-50, a(4)=-43,
     a(5)=535, a(6)=487, a(7)=-4983
   The generating function for the sequence is
   (1 + 3x + 16x² – 8x³ + 36x⁴ – 12x⁵ + x⁶ – x⁷) /
   (1 + 7x² + x⁴)², which leads to the above recurrence. */

seq_t a(idx_t n)
{
    /* Initial eight terms.  Cast to seq_t to avoid narrowing. */
    static const seq_t init[8] = {
        (seq_t)1,
        (seq_t)3,
        (seq_t)2,
        (seq_t)-50,
        (seq_t)-43,
        (seq_t)535,
        (seq_t)487,
        (seq_t)-4983
    };

    if (n < 8)
        return init[n];

    /* We store the last eight values a(i-8) … a(i-1) in q[0] … q[7].
       Initially q holds the eight initial terms a(0) … a(7). */
    seq_t q[8];
    for (idx_t i = 0; i < 8; ++i)
        q[i] = init[i];

    /* Compute a(8) … a(n).  After the loop, q[7] holds a(n). */
    for (idx_t i = 8; i <= n; ++i) {
        seq_t new_val = -( (seq_t)14 * q[6]       /* a(i-2)  */
                          + (seq_t)51 * q[4]       /* a(i-4)  */
                          + (seq_t)14 * q[2]       /* a(i-6)  */
                          +            q[0] );     /* a(i-8)  */

        /* Shift the window: drop the oldest value (q[0]) and
           append the new one to the right. */
        for (int k = 0; k < 7; ++k)
            q[k] = q[k + 1];
        q[7] = new_val;
    }

    return q[7];
}

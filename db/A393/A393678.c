/*
 * A393678  Lower (1/2,1/3) midsequence of (n^2) and ((n+1)^2); see Comments.
 * https://oeis.org/A393678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) - a(n-4) - a(n-5) + a(n-6),
   with a(0)=0, a(1)=1, a(2)=5, a(3)=9, a(4)=16, a(5)=24. */

seq_t a(idx_t n)
{
    static const seq_t init_vals[6] =
    { (seq_t)0, (seq_t)1, (seq_t)5, (seq_t)9, (seq_t)16, (seq_t)24 };

    if (n < 6)
        return init_vals[n];

    /* Circular buffer for the last seven values; index modulo 7. */
    seq_t buf[7];
    buf[0] = (seq_t)0;  /* a(0) */
    buf[1] = (seq_t)1;  /* a(1) */
    buf[2] = (seq_t)5;  /* a(2) */
    buf[3] = (seq_t)9;  /* a(3) */
    buf[4] = (seq_t)16; /* a(4) */
    buf[5] = (seq_t)24; /* a(5) */

    seq_t val;           /* temporary storage for a(i) */
    for (idx_t i = 6; i <= n; ++i) {
        val = buf[(i-1) % 7] + buf[(i-2) % 7]
            - buf[(i-4) % 7] - buf[(i-5) % 7] + buf[(i-6) % 7];
        buf[i % 7] = val;
    }
    return buf[n % 7];
}

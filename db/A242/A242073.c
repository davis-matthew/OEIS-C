/*
 * A242073  a(n) = - (a(n-1)*a(n-4) + a(n-2)*a(n-3)) / a(n-5) with a(0)=1,
 *          a(1)=a(2)=-1, a(3)=-2, a(4)=1.
 * https://oeis.org/A242073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -(a(n-1)*a(n-4) + a(n-2)*a(n-3)) / a(n-5) with a(0)=1,
 *            a(1)=a(2)=-1, a(3)=-2, a(4)=1
 * Range:     i8 n <= 9501, i16 n <= 9501, i32 n <= 9501, i64 n <= 9501,
 *            i128 n <= 9501, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly */
    if (n < 5) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)-1;
            case 2: return (seq_t)-1;
            case 3: return (seq_t)-2;
            case 4: return (seq_t)1;
            default: break; /* never reached */
        }
    }

    /* Sliding window of the last five values:
       w[0] = a(i-1), w[1] = a(i-2), w[2] = a(i-3),
       w[3] = a(i-4), w[4] = a(i-5)  */
    seq_t w[5];
    w[0] = (seq_t)1;   /* a(4) */
    w[1] = (seq_t)-2;  /* a(3) */
    w[2] = (seq_t)-1;  /* a(2) */
    w[3] = (seq_t)-1;  /* a(1) */
    w[4] = (seq_t)1;   /* a(0) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t num = w[0] * w[3] + w[1] * w[2];
        seq_t next = -(num) / w[4];
        /* Shift the window: drop the oldest value */
        w[4] = w[3];
        w[3] = w[2];
        w[2] = w[1];
        w[1] = w[0];
        w[0] = next;
    }
    return w[0];
}

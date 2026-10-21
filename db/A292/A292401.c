/*
 * A292401  p-INVERT of (1,0,2,0,2,0,2,0,2,0,...), where p(S) = (1 - S)^2.
 * https://oeis.org/A292401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 32, u64 n <= 67, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A292401
   p-INVERT of (1,0,2,0,2,0,2,0,2,0,...) with p(S) = (1 - S)^2
   Recurrence: a(n) = 2*a(n-1)+a(n-2)-3*a(n-4)-2*a(n-5)-a(n-6)  for n >= 7
   Initial values:
   a(0)=2, a(1)=3, a(2)=8, a(3)=17, a(4)=34, a(5)=71, a(6)=144
*/
seq_t a(idx_t n) {
    /* Small table of the first seven terms */
    static const seq_t init[7] = {2, 3, 8, 17, 34, 71, 144};

    if (n <= 6) {
        return init[n];
    }

    /* Use a 6‑element ring buffer containing a(i-6)…a(i-1) */
    seq_t last[6];
    last[0] = init[1];  /* a(1)  */
    last[1] = init[2];  /* a(2)  */
    last[2] = init[3];  /* a(3)  */
    last[3] = init[4];  /* a(4)  */
    last[4] = init[5];  /* a(5)  */
    last[5] = init[6];  /* a(6)  */

    for (idx_t i = 7; i <= n; ++i) {
        /* a(i) = 2*a(i-1)+a(i-2)-3*a(i-4)-2*a(i-5)-a(i-6) */
        seq_t cur = 2 * last[5] + last[4]          /* 2*a(i-1)+a(i-2)   */
                           - 3 * last[2] - 2 * last[1] - last[0]; /* -3*a(i-4)-2*a(i-5)-a(i-6) */

        /* shift buffer forward */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = last[5];
        last[5] = cur;
    }

    return last[5];
}

/*
 * A290698  Number of minimal edge covers in the n-antiprism graph.
 * https://oeis.org/A290698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 2*a(n-1) + 5*a(n-2) + 12*a(n-3) + 11*a(n-4) + 5*a(n-5)
 *             - 3*a(n-6) - 4*a(n-7) - a(n-8) + a(n-9)
 *
 * Initial values: 
 * a(1)=2, a(2)=14, a(3)=74, a(4)=286, a(5)=1157,
 * a(6)=4778, a(7)=19623, a(8)=80478, a(9)=330293
 */

seq_t a(idx_t n)
{
    /* handle the small case directly */
    if (n < 10) {
        static const seq_t init_vals[10] = {
            (seq_t)0,          /* dummy for index 0 */
            (seq_t)2, (seq_t)14, (seq_t)74, (seq_t)286, (seq_t)1157,
            (seq_t)4778, (seq_t)19623, (seq_t)80478, (seq_t)330293
        };
        return init_vals[n];
    }

    /* last[0] = a(i-9), last[1] = a(i-8), ..., last[8] = a(i-1) */
    seq_t last[9];
    last[0] = (seq_t)2;           /* a1 */
    last[1] = (seq_t)14;          /* a2 */
    last[2] = (seq_t)74;          /* a3 */
    last[3] = (seq_t)286;         /* a4 */
    last[4] = (seq_t)1157;        /* a5 */
    last[5] = (seq_t)4778;        /* a6 */
    last[6] = (seq_t)19623;       /* a7 */
    last[7] = (seq_t)80478;       /* a8 */
    last[8] = (seq_t)330293;      /* a9 */

    seq_t curr = 0;
    for (idx_t i = 10; i <= n; ++i) {
        /* compute a(i) from the 9 previous values */
        seq_t pos = last[8] * (seq_t)2
                  + last[7] * (seq_t)5
                  + last[6] * (seq_t)12
                  + last[5] * (seq_t)11
                  + last[4] * (seq_t)5;
        seq_t neg = last[3] * (seq_t)3
                  + last[2] * (seq_t)4
                  + last[1];
        curr = pos;
        curr += last[0];
        curr -= neg;          /* the subtraction is safe because the result is non‑negative */

        /* shift the window: drop a(i-9), move others left, insert new a(i) */
        for (int j = 0; j < 8; ++j) 
            last[j] = last[j+1];
        last[8] = curr;
    }
    return curr;
}

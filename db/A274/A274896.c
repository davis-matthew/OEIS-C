/*
 * A274896  Number of 4 X n 0..2 arrays with no element equal to any value at
 *          offset (0,-1) (-1,-1) or (-2,0) and new values introduced in order
 *          0..2.
 * https://oeis.org/A274896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 42, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-1) - 9*a(n-2) + 10*a(n-3) - 6*a(n-4) + a(n-5)
 * Initial values: a(1)=6, a(2)=14, a(3)=38, a(4)=104, a(5)=290, a(6)=815
 */
seq_t a(idx_t n)
{
    static const seq_t init[6] = {6, 14, 38, 104, 290, 815};

    if (n <= 6) {
        /* n is unsigned; guard against n==0 by returning 0 */
        return (n == 0) ? (seq_t)0 : init[n - 1];
    }

    /* Sliding window holding a(i-5)..a(i-1) */
    seq_t v[6];
    for (idx_t i = 0; i < 6; ++i) v[i] = init[i];

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute next term using the recurrence */
        seq_t left  = 5 * v[5] + 10 * v[3] + v[1];   /* 5*a(i-1)+10*a(i-3)+a(i-5) */
        seq_t right = 9 * v[4] + 6 * v[2];            /* 9*a(i-2)+6*a(i-4) */
        seq_t next  = left - right;                  /* a(i) */

        /* Shift the window: drop a(i-5), shift others, push new value */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = next;
    }

    return v[5];  /* v[5] now holds a(n) */
}

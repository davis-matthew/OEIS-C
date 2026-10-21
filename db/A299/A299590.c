/*
 * A299590  Number of n X 3 0..1 arrays with every element unequal to 0, 1 or
 *          4 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A299590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 72, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Empirical recurrence for A299590:
 * a(1)=3, a(2)=3, a(3)=5, a(4)=11, a(5)=19, a(6)=35,
 * a(7)=65, a(8)=120;
 * For n>8: a(n)=a(n-1)+2*a(n-3)+a(n-4)+a(n-5).
 */

seq_t a(idx_t n)
{
    /* Handle the first eight terms directly */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)11;
    if (n == 5) return (seq_t)19;
    if (n == 6) return (seq_t)35;
    if (n == 7) return (seq_t)65;
    if (n == 8) return (seq_t)120;

    /* Cache the last six values needed for the recurrence:
     * prev[0] = a(cur-6), prev[5] = a(cur-1)  (cur will start at 9) */
    seq_t prev[6];
    prev[0] = (seq_t)5;   /* a(3)   */
    prev[1] = (seq_t)11;  /* a(4)   */
    prev[2] = (seq_t)19;  /* a(5)   */
    prev[3] = (seq_t)35;  /* a(6)   */
    prev[4] = (seq_t)65;  /* a(7)   */
    prev[5] = (seq_t)120; /* a(8)   */

    for (idx_t cur = 9; cur <= n; ++cur) {
        /* a(cur) = a(cur-1) + 2*a(cur-3) + a(cur-4) + a(cur-5) */
        seq_t next = prev[5]           /* a(cur-1) */
                   + 2 * prev[3]       /* 2*a(cur-3) */
                   + prev[2]           /* a(cur-4) */
                   + prev[1];          /* a(cur-5) */

        /* Shift the window left by one */
        for (int k = 0; k < 5; ++k) {
            prev[k] = prev[k + 1];
        }
        prev[5] = next;
    }
    return prev[5];  /* a(n) */
}

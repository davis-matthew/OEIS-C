/*
 * A247311  Rectangular array read upwards by columns: T = T(n,k) = number of
 *          paths from (0,1) to (n,k), where 0 <= k <= 2, consisting of
 *          segments given by the vectors (1,1), (1,0), (1,-1).
 * https://oeis.org/A247311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 42, u32 n <= 80, u64 n <= 155,
 *            u128 n <= 306, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute A247311.
    T(n,k) counts paths from (0,1) to (n,k) using steps (1,1),(1,0),(1,-1).
    Recurrence: T(0,0)=1, T(0,1)=T(0,2)=0;
                for n>0: T(n,0)=T(n-1,0)+T(n-1,1),
                         T(n,1)=T(n-1,0)+T(n-1,1)+T(n-1,2),
                         T(n,2)=T(n-1,1)+T(n-1,2).
    The sequence a(n) is read columnwise, top to bottom:
        a(3m)   = T(m,0),   a(3m+1) = T(m,1),   a(3m+2) = T(m,2).   */

seq_t a(idx_t n) {
    /* m = column index, r = position inside the column (0..2) */
    idx_t m = n / 3;
    idx_t r = n - m * 3;   /* n % 3 */

    /* T(0,0)=1, T(0,1)=0, T(0,2)=0 */
    seq_t t0 = (seq_t)1;   /* k = 0 */
    seq_t t1 = (seq_t)0;   /* k = 1 */
    seq_t t2 = (seq_t)0;   /* k = 2 */

    /* iterate from step 1 to m to reach column m */
    for (idx_t i = 1; i <= m; ++i) {
        seq_t nt0 = t0 + t1;                 /* k = 0 */
        seq_t nt1 = t0 + t1 + t2;             /* k = 1 */
        seq_t nt2 = t1 + t2;                 /* k = 2 */
        t0 = nt0;
        t1 = nt1;
        t2 = nt2;
    }

    /* output according to position inside the column */
    switch (r) {
        case 0: return t0;   /* top row */
        case 1: return t1;   /* middle row */
        case 2: return t2;   /* bottom row */
        default: return (seq_t)0; /* unreachable */
    }
}

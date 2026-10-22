/*
 * A365967  a(2*n) = A030186(n), a(2*n+1) = A033505(n).
 * https://oeis.org/A365967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) + a(n-4) - a(n-6) (n>=6) with initial values
 *            a(0)=1,a(1)=1,a(2)=2,a(3)=3,a(4)=7,a(5)=10
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* handle small n explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)7;
    if (n == 5) return (seq_t)10;

    /* circular buffer holding the last six values a(i-6)...a(i-1) */
    seq_t prev[6];
    prev[0] = (seq_t)1; /* a(0) */
    prev[1] = (seq_t)1; /* a(1) */
    prev[2] = (seq_t)2; /* a(2) */
    prev[3] = (seq_t)3; /* a(3) */
    prev[4] = (seq_t)7; /* a(4) */
    prev[5] = (seq_t)10;/* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        /* compute a(i) using the recurrence */
        seq_t val =
            (seq_t)3 * prev[(i - 2) % 6]        /* 3*a(i-2) */
          + prev[(i - 4) % 6]                  /* +a(i-4) */
          - prev[(i - 6) % 6];                 /* -a(i-6) */
        prev[i % 6] = val; /* store a(i) for future use */
    }

    return prev[n % 6];
}

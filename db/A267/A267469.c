/*
 * A267469  Number of length-n 0..6 arrays with no following elements larger
 *          than the first repeated value.
 * https://oeis.org/A267469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 34*a(n-1) - 490*a(n-2) + 3892*a(n-3) - 18529*a(n-4) +
 *            53746*a(n-5) - 91860*a(n-6) + 83448*a(n-7) - 30240*a(n-8)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first eight terms directly. */
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)49;
    if (n == 3) return (seq_t)322;
    if (n == 4) return (seq_t)2072;
    if (n == 5) return (seq_t)13216;
    if (n == 6) return (seq_t)83972;
    if (n == 7) return (seq_t)532840;
    if (n == 8) return (seq_t)3381860;

    /* Use a circular buffer holding the last eight values. */
    seq_t arr[8];
    arr[0] = (seq_t)7;       /* a(1) */
    arr[1] = (seq_t)49;      /* a(2) */
    arr[2] = (seq_t)322;     /* a(3) */
    arr[3] = (seq_t)2072;    /* a(4) */
    arr[4] = (seq_t)13216;   /* a(5) */
    arr[5] = (seq_t)83972;   /* a(6) */
    arr[6] = (seq_t)532840;  /* a(7) */
    arr[7] = (seq_t)3381860; /* a(8) */

    /* Iterate from k = 9 up to the requested n. */
    for (idx_t k = 9; k <= n; ++k) {
        seq_t term = (seq_t)34   * arr[7]
                   + (seq_t)3892 * arr[5]
                   + (seq_t)53746 * arr[3]
                   + (seq_t)83448 * arr[1];
        term -= (seq_t)490   * arr[6];
        term -= (seq_t)18529 * arr[4];
        term -= (seq_t)91860 * arr[2];
        term -= (seq_t)30240 * arr[0];

        /* Shift the buffer left by one position. */
        arr[0] = arr[1];
        arr[1] = arr[2];
        arr[2] = arr[3];
        arr[3] = arr[4];
        arr[4] = arr[5];
        arr[5] = arr[6];
        arr[6] = arr[7];
        arr[7] = term;
    }

    /* After the loop, arr[7] holds a(n). */
    return arr[7];
}

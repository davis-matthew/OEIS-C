/*
 * A249885  Number of length 2+6 0..n arrays with no seven consecutive terms
 *          having the maximum of any three terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A249885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9), with
 *            initial values: a(1)=35, a(2)=1365, a(3)=18390, a(4)=136010,
 *            a(5)=684585, a(6)=2644815, a(7)=8435180, a(8)=23274900,
 *            a(9)=57355695
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Initial values */
    static const seq_t init[9] = {
        (seq_t)35ULL,
        (seq_t)1365ULL,
        (seq_t)18390ULL,
        (seq_t)136010ULL,
        (seq_t)684585ULL,
        (seq_t)2644815ULL,
        (seq_t)8435180ULL,
        (seq_t)23274900ULL,
        (seq_t)57355695ULL
    };

    if (n <= 9)
        return init[n - 1];

    /* Sliding window of the last 9 terms:
       arr[0] = a(k-8), arr[1] = a(k-7), ..., arr[8] = a(k)  */
    seq_t arr[9];
    for (int i = 0; i < 9; ++i)
        arr[i] = init[i];

    for (idx_t k = 10; k <= n; ++k) {
        /* Compute next term using non‑negative intermediate sums */
        seq_t pos = 9 * arr[8]          /* a(k-1)  */
                  + 84 * arr[6]          /* a(k-3)  */
                  + 126 * arr[4]         /* a(k-5)  */
                  + 36 * arr[2]          /* a(k-7)  */
                  + arr[0];             /* a(k-9)  */

        seq_t neg = 36 * arr[7]          /* a(k-2)  */
                  + 126 * arr[5]         /* a(k-4)  */
                  + 84 * arr[3]          /* a(k-6)  */
                  + 9 * arr[1];          /* a(k-8)  */

        seq_t next = pos - neg;          /* a(k)    */

        /* Shift left to discard a(k-9) and append a(k) */
        for (int i = 0; i < 8; ++i)
            arr[i] = arr[i + 1];
        arr[8] = next;
    }

    return arr[8];
}

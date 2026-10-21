/*
 * A268762  Number of n X 4 binary arrays with some element plus some
 *          horizontally, vertically, diagonally or antidiagonally adjacent
 *          neighbor totalling two exactly once.
 * https://oeis.org/A268762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 10*a(n-2) - 32*a(n-3) - 47*a(n-4) + 40*a(n-5)
 *            + 38*a(n-6) - 12*a(n-7) - 9*a(n-8), a(1) = 5, a(2) = 44, a(3) =
 *            223, a(4) = 1148, a(5) = 5170, a(6) = 23156, a(7) = 99057, a(8)
 *            = 418924
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small indices explicitly. */
    switch (n) {
        case 1: return (seq_t)5;
        case 2: return (seq_t)44;
        case 3: return (seq_t)223;
        case 4: return (seq_t)1148;
        case 5: return (seq_t)5170;
        case 6: return (seq_t)23156;
        case 7: return (seq_t)99057;
        case 8: return (seq_t)418924;
    }

    /* For n >= 9, compute iteratively using a rolling window of the last
       eight terms. */
    seq_t window[8] = {(seq_t)5, (seq_t)44, (seq_t)223, (seq_t)1148,
                       (seq_t)5170, (seq_t)23156, (seq_t)99057,
                       (seq_t)418924};

    for (idx_t i = 9; i <= n; ++i) {
        /* a(i) = 4*a(i-1) + 10*a(i-2) - 32*a(i-3) - 47*a(i-4) +
                  40*a(i-5) + 38*a(i-6) - 12*a(i-7) - 9*a(i-8) */
        seq_t val = (seq_t)4 * window[7]
                  + (seq_t)10 * window[6]
                  - (seq_t)32 * window[5]
                  - (seq_t)47 * window[4]
                  + (seq_t)40 * window[3]
                  + (seq_t)38 * window[2]
                  - (seq_t)12 * window[1]
                  - (seq_t)9  * window[0];

        /* Shift the window: discard a(i-8), shift others left,
           and insert a(i) at the end. */
        window[0] = window[1];
        window[1] = window[2];
        window[2] = window[3];
        window[3] = window[4];
        window[4] = window[5];
        window[5] = window[6];
        window[6] = window[7];
        window[7] = val;
    }

    return window[7];
}

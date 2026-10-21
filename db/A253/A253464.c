/*
 * A253464  Number of (n+1) X (3+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal maximum minus antidiagonal minimum unequal to its
 *          neighbors horizontally, vertically, diagonally and antidiagonally.
 * https://oeis.org/A253464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-2) - 4*a(n-4) for n>6; a(1) = 3640, a(2) = 1400,
 *            a(3) = 2000, a(4) = 2600, a(5) = 5000, a(6) = 7784
 * Range:     u8 none, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _a_init[6] = {
    (seq_t)3640,
    (seq_t)1400,
    (seq_t)2000,
    (seq_t)2600,
    (seq_t)5000,
    (seq_t)7784
};

seq_t a(idx_t n) {
    if (n <= 6)
        return _a_init[n - 1];

    /* arr[0] = a(i-4), arr[1] = a(i-3), arr[2] = a(i-2), arr[3] = a(i-1) */
    seq_t arr[4] = {
        _a_init[2], /* a(3) */
        _a_init[3], /* a(4) */
        _a_init[4], /* a(5) */
        _a_init[5]  /* a(6) */
    };

    for (idx_t i = 7; i <= n; ++i) {
        seq_t new_val = 5 * arr[2] - 4 * arr[0];
        arr[0] = arr[1];
        arr[1] = arr[2];
        arr[2] = arr[3];
        arr[3] = new_val;
    }
    return arr[3];
}

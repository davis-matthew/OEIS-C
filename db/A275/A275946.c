/*
 * A275946  Number of nonzero digits that are the sole occupants of their
 *          slope in factorial base representation: a(n) =
 *          A056169(A275734(n)). (See comments for more exact definition.)
 * https://oeis.org/A275946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of nonzero digits that are sole occupants of their
 *            * slope in the factorial base representation of n
 * Range:     u8 n <= 9798, u16 n <= 9798, u32 n <= 9798, u64 n <= 9798,
 *            u128 n <= 9798, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t tmp = n;
    int i = 1;                     /* factorial index (1! is the least significant) */
    int slopes[64];                /* slopes of the nonzero digits, will hold at most ~20 entries */
    int k = 0;                     /* number of slopes stored */

    /* Determine the factorial base representation and record slopes */
    while (tmp != 0) {
        idx_t base = i + 1;              /* next factorial base (i+1)! divided by i! */
        idx_t digit = tmp % base;        /* digit at position i */
        tmp /= base;                     /* remaining value */
        if (digit != 0) {
            int slope = i - (int)digit;  /* i - d_x */
            slopes[k++] = slope;
        }
        ++i;
    }

    /* Count slopes that occur exactly once */
    int uniq = 0;
    for (int p = 0; p < k; ++p) {
        int cnt = 0;
        for (int q = 0; q < k; ++q)
            if (slopes[p] == slopes[q])
                ++cnt;
        if (cnt == 1)
            ++uniq;
    }

    return (seq_t)uniq;
}

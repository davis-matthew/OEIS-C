/*
 * A275437  Triangle read by rows: T(n,k) is the number of 01-avoiding binary
 *          words of length n having degree of asymmetry equal to k (n >= 0; 0
 *          <= k <= floor(n/2)).
 * https://oeis.org/A275437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = 1 if the linear index n corresponds to the last element *
 *            of an even row in the triangle; otherwise a(n) = 2. * The
 *            triangle rows have lengths 1 + floor(m/2) for row index m ≥ 0. *
 *            In row m: T(m, k) = 1 when m is even and k == m/2; otherwise
 *            T(m, k) = 2
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = 0;          /* current row */
    idx_t cum = 0;        /* number of elements before row m */
    idx_t len;            /* length of current row */

    /* Find the row m such that cumulative count <= n < cumulative + len */
    for (;;) {
        len = 1 + m/2;
        if (cum + len > n)
            break;
        cum += len;
        ++m;
    }

    idx_t k = n - cum;    /* column index within row m */

    /* Check if we are at the last element of an even row */
    if ((m & 1u) == 0u && k == m/2)
        return (seq_t)1;
    else
        return (seq_t)2;
}

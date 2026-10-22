/*
 * A301794  Number of 5Xn 0..1 arrays with every element equal to 0 or 1
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=16, a(2)=89, a(3)=48, a(4)=61, a(5)=82, a(n) = 2*a(n-1) -
 *            a(n-3) for n>5
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* guard against calls with an index below the offset */
    if (n < 1)
        return (seq_t)0;

    /* initial values */
    static const seq_t init_vals[6] = {0, 16, 89, 48, 61, 82};

    if (n <= 5)
        return init_vals[n];

    /* iterate the recurrence from 6 up to the requested n */
    seq_t a_nm3 = init_vals[3];  /* a(n-3) */
    seq_t a_nm2 = init_vals[4];  /* a(n-2) */
    seq_t a_nm1 = init_vals[5];  /* a(n-1) */
    seq_t a_n;

    for (idx_t i = 6; i <= n; ++i) {
        a_n = ((seq_t)2 * a_nm1) - a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}

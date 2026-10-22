/*
 * A395902  Triple convolution of the modified Fibonacci sequence F(3n)/2
 *          (A001076).
 * https://oeis.org/A395902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A395902: a(n) = 12*a(n-1) - 45*a(n-2) + 40*a(n-3) + 45*a(n-4) +
 *            * 12*a(n-5) + a(n-6) for n >= 6, with a(0..5)=0,0,0,1,12,99
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for n = 0..5 */
    if (n <= 5) {
        static const seq_t base[6] = {0, 0, 0, 1, 12, 99};
        return base[n];
    }

    /* Working variables hold a(n-6) ... a(n-1) */
    seq_t a_nm6 = 0;  /* a(0)    */
    seq_t a_nm5 = 0;  /* a(1)    */
    seq_t a_nm4 = 0;  /* a(2)    */
    seq_t a_nm3 = 1;  /* a(3)    */
    seq_t a_nm2 = 12; /* a(4)    */
    seq_t a_nm1 = 99; /* a(5)    */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t a_n = 12 * a_nm1
                  - 45 * a_nm2
                  + 40 * a_nm3
                  + 45 * a_nm4
                  + 12 * a_nm5
                  +  a_nm6;

        /* Shift the window: drop a_nm6, push a_n */
        a_nm6 = a_nm5;
        a_nm5 = a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}

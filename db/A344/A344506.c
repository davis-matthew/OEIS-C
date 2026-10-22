/*
 * A344506  a(n) = [x^n] 2 / (1 - 7*x + sqrt(1 - 2*x - 3*x^2)).
 * https://oeis.org/A344506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (39*(2 - n)*a(n-3) - (17*n + 5)*a(n-2) + (19*n +
 *            10)*a(n-1))/(3*n + 3) Initial values: a(0)=1, a(1)=4, a(2)=17
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)17;

    seq_t a_nm3 = (seq_t)1;      /* a(n-3) */
    seq_t a_nm2 = (seq_t)4;      /* a(n-2) */
    seq_t a_nm1 = (seq_t)17;     /* a(n-1) */
    seq_t a_n   = 0;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t big   = ((seq_t)(19 * i + 10)) * a_nm1;
        seq_t term2 = ((seq_t)(39 * (i - 2))) * a_nm3;
        seq_t term3 = ((seq_t)(17 * i + 5))  * a_nm2;
        seq_t small = term2 + term3;
        seq_t num   = big - small;
        seq_t denom = (seq_t)(3 * i + 3);

        a_n = num / denom;

        /* shift window for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}

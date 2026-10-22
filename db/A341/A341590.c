/*
 * A341590  a(n) = (Sum_{j=1..3} StirlingS1(3,j)*(2^j-1)^n)/3!.
 * https://oeis.org/A341590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 31*a(n-2) + 21*a(n-3); Initial values: a(0) =
 *            0, a(1) = 0, a(2) = 4 (Also matches the closed form a(n) = (7^n
 *            - 3^(n+1) + 2)/6)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)4;

    seq_t a_nm3 = (seq_t)0;  // a(n-3)
    seq_t a_nm2 = (seq_t)0;  // a(n-2)
    seq_t a_nm1 = (seq_t)4;  // a(n-1) for n=3

    seq_t a_n;                 // will hold a(n)
    for (idx_t i = 3; i <= n; ++i) {
        a_n = (a_nm1 * (seq_t)11) - (a_nm2 * (seq_t)31) + (a_nm3 * (seq_t)21);
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}

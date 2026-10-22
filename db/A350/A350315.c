/*
 * A350315  Length of the rows of the Redstone permutation A350313.
 * https://oeis.org/A350315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n >= 4 : a(n) = 2 * b(n-4) + 6, where b(m) is periodic with
 *            period 34 and b(m) = -c[m mod 34] for coefficients c given below
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 2;
    if (n == 2) return 3;
    if (n == 3) return 6;

    /* Coefficients of the numerator of the generating function for b(n) */
    static const int coeff[34] = {
        0, 0, 0, -1, 1, 0, 0, 0,
       -1, 1, 0, 0, 0, -1, 1, 0,
        0, 0, -1, 1, 0, 0, 0, -1,
        1, 0, 0, 0, -1, 1, 0, 0,
        0, -3
    };

    idx_t m = n - 4;               /* index for b(...) */
    int c = coeff[m % 34];         /* coefficient from numerator */
    int b = -c;                    /* b(m) = -c */
    return (seq_t)(b * 2 + 6);
}

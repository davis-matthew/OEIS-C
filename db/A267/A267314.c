/*
 * A267314  Expansion of 2*x*(1 + 2*x - x^2)/((1 - x)*(1 + x^2)^2).
 * https://oeis.org/A267314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 - (-1)^(n*(n+1)/2) - (1+(-1)^n)*(-1)^(n/2)*n
 * Range:     i8 n <= 65, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;                     /* start with 1 */
    /* Compute (-1)^(n*(n+1)/2) using n mod 4 */
    int n_mod4 = n & 3;
    seq_t neg1_power_t;
    switch (n_mod4) {
        case 0: /* n ≡ 0 (mod 4) => exponent even */
        case 3: /* n ≡ 3 (mod 4) => exponent even */
            neg1_power_t = 1;
            break;
        case 1: /* n ≡ 1 (mod 4) => exponent odd */
        case 2: /* n ≡ 2 (mod 4) => exponent odd */
            neg1_power_t = -1;
            break;
    }
    result -= neg1_power_t;               /* subtract A */

    /* Compute (1+(-1)^n)*(-1)^(n/2)*n term only when n is even   */
    if ((n & 1) == 0) {                   /* n even */
        idx_t m = n >> 1;                 /* n/2 */
        seq_t double_n = (seq_t)n << 1;   /* 2*n */
        /* (-1)^(n/2) depends on parity of m */
        seq_t B;
        if ((m & 1) == 0) {               /* m even => (-1)^(n/2) = 1 */
            B = double_n;
        } else {                          /* m odd => (-1)^(n/2) = -1 */
            B = -double_n;
        }
        result -= B;                      /* subtract B */
    }

    return result;
}

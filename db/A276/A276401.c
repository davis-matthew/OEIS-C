/*
 * A276401  If n mod 3 = 0 then a(n) = 3^(n/3) + 12*n, if n mod 3 = 1 then
 *          a(n) = 4*3^((n-4)/3) + 12*n + 51, otherwise a(n) = 2*3^((n-2)/3) +
 *          12*n - 36.
 * https://oeis.org/A276401
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   if n mod 3 = 0 then a(n) = 3^(n/3) + 12*n if n mod 3 = 1 then
 *            a(n) = 4*3^((n-4)/3) + 12*n + 51 otherwise a(n) = 2*3^((n-2)/3)
 *            + 12*n - 36 (offset 3: a(3) is the first term)
 * Range:     u8 n <= 12, u16 n <= 30, u32 n <= 60, u64 n <= 121,
 *            u128 n <= 242, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow3(idx_t e) {
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i) r *= 3;
    return r;
}

seq_t a(idx_t n) {
    /* Handle n less than offset – behaviour is irrelevant per requirements */
    if (n < 3) return 0;

    idx_t mod = n % 3;
    seq_t term;
    if (mod == 0) {
        term = pow3(n / 3);
    } else if (mod == 1) {
        term = pow3((n - 4) / 3);
        term *= 4;
    } else { /* mod == 2 */
        term = pow3((n - 2) / 3);
        term *= 2;
    }

    /* Add the linear part */
    term += ((seq_t)n) * 12;

    /* Adjust constant terms */
    if (mod == 1) {
        term += 51;
    } else if (mod == 2) {
        term -= 36;
    }

    return term;
}

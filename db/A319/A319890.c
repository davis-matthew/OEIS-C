/*
 * A319890  a(n) = 7*6*5*4*3*2*1 - 14*13*12*11*10*9*8 + 21*20*19*18*17*16*15 -
 *          ... + (up to the n-th term).
 * https://oeis.org/A319890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = if n mod 7 != 0: (-1)^{⌊n/7⌋} * falling_fact(n - (n mod
 *            7) + 7, n mod 7) + ∑_{m=1}^{⌊n/7⌋} (-1)^{m+1} *
 *            falling_fact(7*m, 7) where falling_fact(x, k) = x·(x-1)…(x-k+1)
 * Range:     i8 n <= 2, i16 n <= 11, i32 n <= 27, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A319890. */
seq_t a(idx_t n)
{
    const idx_t k = 7;
    idx_t blocks = n / k;          /* ⌊n/7⌋ */
    idx_t r = n % k;               /* n mod 7 */

    seq_t term1 = 0;
    if (r != 0) {
        /* sign = (-1)^{blocks} */
        seq_t sign1 = ((blocks & 1) == 0) ? 1 : -1;
        seq_t start = (seq_t)n - (seq_t)r + (seq_t)k;   /* n - r + 7 */
        seq_t prod = 1;
        for (idx_t i = 0; i < r; ++i) {
            prod *= (start - (seq_t)i);
        }
        term1 = sign1 * prod;
    }

    seq_t term2 = 0;
    for (idx_t m = 1; m <= blocks; ++m) {
        /* sign = (-1)^{m+1} */
        seq_t sign2 = ((m & 1) == 1) ? 1 : -1;
        seq_t j = (seq_t)m * (seq_t)k;   /* j = 7*m */
        seq_t prod = 1;
        for (idx_t i = 0; i < k; ++i) {
            prod *= (j - (seq_t)i);
        }
        term2 += sign2 * prod;
    }

    return term1 + term2;
}

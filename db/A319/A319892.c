/*
 * A319892  a(n) = 9*8*7*6*5*4*3*2*1 - 18*17*16*15*14*13*12*11*10 +
 *          27*26*25*24*23*22*21*20*19 - ... + (up to the n-th term).
 * https://oeis.org/A319892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^⌊n/9⌋ * Σ_{j=1..8} (⌊(n-j)/9⌋−⌊(n-j-1)/9⌋) *
 *            Π_{i=1..j} (n−i−j+9+1) + Σ_{m=1..⌊n/9⌋} (-1)^{m+1} * Π_{i=1..9}
 *            (m*9 - i + 1)
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 17, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t K = 9;
    seq_t total = 0;

    /* First part of the formula */
    idx_t block = n / K;                /* ⌊n/9⌋  */
    seq_t sign1 = (block & 1U) ? (seq_t)-1 : (seq_t)1;
    seq_t sum1  = 0;

    for (idx_t j = 1; j < K; ++j) {
        /* The factor [⌊(n-j)/9⌋−⌊(n-j-1)/9⌋] is 1 iff (n-j) is a multiple of 9  */
        if ((n - j) % K == 0) {
            seq_t prod = 1;
            for (idx_t i = 1; i <= j; ++i) {
                prod *= (seq_t)(n - i - j + K + 1);
            }
            sum1 += prod;
        }
    }
    total += sign1 * sum1;

    /* Second part: sum of complete 9‑term block products */
    for (idx_t m = 1; m * K <= n; ++m) {
        seq_t prod = 1;
        /* Product of the integers m*9, m*9-1, …, m*9-8  */
        for (idx_t i = 1; i <= K; ++i) {
            prod *= (seq_t)(m * K - i + 1);
        }
        seq_t sign2 = (m & 1U) ? (seq_t)1 : (seq_t)-1;   /* (-1)^(m+1) */
        total += sign2 * prod;
    }

    return total;
}

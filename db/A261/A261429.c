/*
 * A261429  Number of permutations p of [3n] without fixed points such that
 *          p^9 = Id.
 * https://oeis.org/A261429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3n)! * [x^(3n)] exp(x^3/3 + x^9/9)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t t = 1;

    /* t0 = product_{i=1..n} (3i-1)(3i-2) = (3n)! / (3^n n!) */
    for (idx_t i = 1; i <= n; ++i) {
        t = t * ((seq_t)3 * i - 1) * ((seq_t)3 * i - 2);
    }
    sum += t;

    /* Subsequent terms via recurrence */
    idx_t maxj = n / 3;
    for (idx_t j = 0; j < maxj; ++j) {
        seq_t num = 3 * (seq_t)(n - 3 * j) * (seq_t)(n - 3 * j - 1) * (seq_t)(n - 3 * j - 2);
        seq_t den = (seq_t)(j + 1);
        t = t * num / den;
        sum += t;
    }
    return sum;
}

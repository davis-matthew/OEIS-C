/*
 * A364102  Sum of divisors of 5*n-3 of form 5*k+4.
 * https://oeis.org/A364102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of divisors d of (5*n-3) with d ≡ 4 (mod 5)
 * Range:     u8 n <= 53, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the value of A364102 at index n (1‑based). */
seq_t a(idx_t n) {
    /* Compute m = 5*n - 3. Use seq_t arithmetic to avoid overflow. */
    seq_t m = ((seq_t)n * 5) - 3;
    seq_t sum = 0;

    /* Enumerate divisors of m via trial division up to sqrt(m). */
    for (seq_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            seq_t d1 = i;
            seq_t d2 = m / i;

            if (d1 % 5 == 4) {
                sum += d1;
            }
            if (d2 != d1 && d2 % 5 == 4) {
                sum += d2;
            }
        }
    }
    return sum;
}

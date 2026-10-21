/*
 * A242962  a(1) = a(2) = 0; for n >= 3: a(n) = (n*(n+1)/2) mod antisigma(n) =
 *          A000217(n) mod A024816(n).
 * https://oeis.org/A242962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000217(n) mod A024816(n) (triangular number modulo sum
 *            of non‑divisors)
 * Range:     u8 n <= 22, u16 n <= 4095, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t sigma_of_idx(idx_t n) {
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            sum += (seq_t)i;
            idx_t j = n / i;
            if (j != i)
                sum += (seq_t)j;
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    if (n <= 2)
        return 0;

    seq_t T = (seq_t)n * ((seq_t)n + 1) / 2;      /* A000217(n) */
    seq_t sigma = sigma_of_idx(n);                /* A000203(n) (sum of divisors) */
    seq_t antisigma = T - sigma;                  /* A024816(n) = T - sigma(n) */

    if (antisigma == 0)
        return 0;                                 /* only for n = 1, 2 which are handled above */

    return T % antisigma;
}

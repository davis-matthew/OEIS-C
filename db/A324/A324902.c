/*
 * A324902  The 2-adic valuation of A318456(n), where A318456(n) = n OR
 *          (sigma(n)-n).
 * https://oeis.org/A324902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = valuation of 2 of [ n OR (σ(n) – n) ], where σ(n) is the
 *            sum of positive divisors of n
 * Range:     u8 n <= 9910, u16 n <= 9910, u32 n <= 9910, u64 n <= 9910,
 *            u128 n <= 9910, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 2‑adic valuation: number of trailing zero bits of x */
static inline seq_t v2(seq_t x) {
    seq_t v = 0;
    while ((x & 1) == 0) {
        x >>= 1;
        v++;
    }
    return v;
}

seq_t a(idx_t n) {
    /* Work with n as an ordinary unsigned integer */
    idx_t N = n;
    /* low bits of n */
    seq_t tn = (seq_t)N;

    /* Compute σ(n) using unsigned 64‑bit arithmetic;
       overflow preserves the low 64 bits, which are all that are needed
       for the 2‑adic valuation of the final expression. */
    unsigned long long sigma_ull = 0;
    for (idx_t i = 1; i <= N / i; ++i) {
        if (N % i == 0) {
            sigma_ull += i;
            idx_t j = N / i;
            if (j != i) sigma_ull += j;
        }
    }

    /* low bits of σ(n) – n (difference) */
    seq_t diff = (seq_t)(sigma_ull - (unsigned long long)N);

    /* n OR (σ(n) – n) in low bits */
    seq_t x = tn | diff;

    /* Return the 2‑adic valuation */
    return v2(x);
}

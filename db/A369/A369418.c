/*
 * A369418  Expansion of o.g.f. x^3/((1-2*x)^2*(1-3*x)^3).
 * https://oeis.org/A369418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-1)*(n^2 - 11*n + 42)/2 - 2^n*(n + 7)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fast exponentiation: base^exp for seq_t base and idx_t exponent */
static inline seq_t pow_uint(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Initial values */
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 0;
    if (n == 3) return 1;
    if (n == 4) return 13;

    /* n^2 - 11*n + 42 */
    seq_t nseq = (seq_t)n;
    seq_t n2 = nseq * nseq;
    seq_t poly = n2 - ((seq_t)11 * nseq) + (seq_t)42;

    /* 3^(n-1) * poly / 2 */
    seq_t term1 = pow_uint((seq_t)3, n - 1) * poly;
    term1 = term1 / 2;

    /* 2^n * (n + 7) */
    seq_t term2 = pow_uint((seq_t)2, n) * ((seq_t)n + 7);

    return term1 - term2;
}

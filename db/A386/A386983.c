/*
 * A386983  E.g.f. A(x) satisfies A(x) = exp(x^2 * (1+x)^2 * A(x)).
 * https://oeis.org/A386983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (k+1)^(k-1) * binomial(2k,
 *            n-2k) / k!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

static seq_t binomial(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (seq_t)(n - k + i);
        res /= i;
    }
    return res;
}

seq_t a(idx_t n) {
    /* compute n! */
    seq_t factorial_n = 1;
    for (idx_t i = 1; i <= n; ++i) factorial_n *= (seq_t)i;

    seq_t P = factorial_n;   /* P = n! / k!  (initially k=0) */
    seq_t sum = 0;
    idx_t maxk = n / 2;

    for (idx_t k = 0; k <= maxk; ++k) {
        if (k > 0) P /= k;   /* update P = n! / k! */

        seq_t pow_term;
        if (k == 0) {
            pow_term = 1;          /* (1)^(-1) treated as 1 for k=0 */
        } else {
            pow_term = pow_seq((seq_t)(k + 1), k - 1);
        }

        idx_t m = n - 2 * k;
        if (m > 2 * k) continue;   /* binomial would be 0 */

        seq_t bin = binomial(2 * k, m);
        if (bin == 0) continue;

        seq_t term = pow_term;
        term *= bin;
        term *= P;
        sum += term;
    }
    return sum;
}

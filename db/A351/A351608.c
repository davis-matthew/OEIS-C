/*
 * A351608  a(n) = n^10 * Sum_{d^2|n} 1 / d^10.
 * https://oeis.org/A351608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n^10 * Sum_{d^2|n} 1/d^10
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 84, u128 n <= 7131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, unsigned exp) {
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return res;
}

static seq_t term_for_prime(idx_t p, unsigned e) {
    seq_t pseq = (seq_t)p;
    seq_t p10 = 1;
    for (unsigned i = 0; i < 10; ++i) p10 *= pseq;           /* p^10   */
    seq_t p10e = pow_seq(p10, e);                            /* p^(10e) */
    unsigned f = (e > 0) ? ((e - 1) / 2) : 0;                /* floor((e-1)/2) */
    seq_t p10f = pow_seq(p10, f);                            /* p^(10f) */
    seq_t numer = p10e - p10f;                               /* p^(10e)-p^(10f) */
    seq_t denom = p10 - (seq_t)1;                            /* p^10-1   */
    seq_t term = p10 * (numer / denom);                     /* (p^10)*(numer/denom) */
    return term;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* defined only for n ≥ 1 */
    idx_t temp = n;
    seq_t result = 1;

    /* factor 2 */
    unsigned e = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++e;
    }
    if (e > 0) result *= term_for_prime(2, e);

    /* factor odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            unsigned exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            result *= term_for_prime(p, exp);
        }
    }

    /* remaining prime factor > 1 */
    if (temp > 1) {
        result *= term_for_prime(temp, 1);
    }

    return result;
}

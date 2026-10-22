/*
 * A390554  a(n) is the sum of the bi-unitary divisors of n^2.
 * https://oeis.org/A390554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product over primes p^e dividing n of (sum_{i=0}^{2e} p^i
 *            - p^e)
 * Range:     u8 n <= 11, u16 n <= 179, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the multiplicative form of the sum of bi‑unitary divisors of n^2. */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined for 0; not used by the caller */
        return (seq_t)0;

    idx_t tmp = n;
    seq_t result = (seq_t)1;

    /* handle factor 2 separately for speed */
    idx_t exp = 0;
    while ((tmp & 1) == 0) {      /* tmp % 2 == 0 */
        tmp >>= 1;
        ++exp;
    }
    if (exp > 0) {
        seq_t p = (seq_t)2;
        seq_t sum = (seq_t)1;      /* p^0 */
        seq_t power = (seq_t)1;
        for (idx_t i = 1; i <= 2 * exp; ++i) {
            power *= p;
            sum += power;
        }
        seq_t p_power_e = (seq_t)1;
        for (idx_t i = 0; i < exp; ++i) p_power_e *= p;
        seq_t term = sum - p_power_e;
        result *= term;
    }

    /* odd prime factors */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p != 0) continue;
        exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        seq_t p_seq = (seq_t)p;
        seq_t sum = (seq_t)1;
        seq_t power = (seq_t)1;
        for (idx_t i = 1; i <= 2 * exp; ++i) {
            power *= p_seq;
            sum += power;
        }
        seq_t p_power_e = (seq_t)1;
        for (idx_t i = 0; i < exp; ++i) p_power_e *= p_seq;
        seq_t term = sum - p_power_e;
        result *= term;
    }

    /* if a leftover prime factor remains */
    if (tmp > 1) {
        idx_t exp1 = 1;
        seq_t p_seq = (seq_t)tmp;
        seq_t sum = (seq_t)1;
        seq_t power = (seq_t)1;
        for (idx_t i = 1; i <= 2 * exp1; ++i) {
            power *= p_seq;
            sum += power;
        }
        seq_t p_power_e = p_seq;  /* p^1 */
        seq_t term = sum - p_power_e;
        result *= term;
    }

    return result;
}

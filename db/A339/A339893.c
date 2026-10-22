/*
 * A339893  a(n) = A000523(n) - A001222(n); floor(log_2(n)) minus the number
 *          of prime divisors of n, counted with multiplicity.
 * https://oeis.org/A339893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log_2(n)) - A001222(n)
 * Range:     u8 n <= 9438, u16 n <= 9438, u32 n <= 9438, u64 n <= 9438,
 *            u128 n <= 9438, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A001222 counts the number of prime factors of n, counted with multiplicity. */
seq_t a(idx_t n)
{
    /* compute floor(log_2(n)) */
    seq_t log2 = 0;
    idx_t t = n;
    while (t >>= 1) {
        ++log2;
    }

    /* compute the number of prime factors with multiplicity (bigomega) */
    seq_t omega = 0;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p) {
        while (m % p == 0) {
            ++omega;
            m /= p;
        }
    }
    if (m > 1) {          /* m itself is a prime factor > sqrt(original n) */
        ++omega;
    }

    return log2 - omega;
}

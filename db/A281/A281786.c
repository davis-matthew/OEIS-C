/*
 * A281786  Expansion of a(q) * b(q^2) + a(q^2) * b(q) in powers of q where
 *          a(), b() are cubic AGM functions.
 * https://oeis.org/A281786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 3*b(n) for n > 0, a(0)=2. b is multiplicative with b(2^e)
 *            = 1, b(3^e) = -8 (e > 0), b(p^e) = (p^(e+1)-1)/(p-1) for primes
 *            p > 3
 * Range:     i8 n <= 12, i16 n <= 541, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;          /* a(0) = 2 */
    if (n == 1) return (seq_t)3;          /* a(1) = 3, computed via b(1)=1 */

    seq_t prod = (seq_t)1;                /* b(n) will be accumulated in prod */
    idx_t m = n;

    /* Trial division for prime factors up to sqrt(m) */
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if (p == 2) {
                /* b(2^e) = 1 → no change to prod */
            } else if (p == 3) {
                prod = prod * (seq_t)(-8);
            } else {
                /* b(p^e) = (p^(e+1)-1)/(p-1) */
                seq_t num = (seq_t)1;
                for (int i = 0; i < exp + 1; ++i) num *= (seq_t)p;
                seq_t denom = (seq_t)(p - 1);
                seq_t term = num / denom;
                prod = prod * term;
            }
        }
    }

    /* Remaining prime factor, if any */
    if (m > 1) {
        if (m == 2) {
            /* no effect */
        } else if (m == 3) {
            prod = prod * (seq_t)(-8);
        } else {
            seq_t num = (seq_t)m * (seq_t)m;  /* m^(1+1) */
            seq_t denom = (seq_t)(m - 1);
            seq_t term = num / denom;
            prod = prod * term;
        }
    }

    /* a(n) = 3 * b(n) for n > 0 */
    return prod * (seq_t)3;
}

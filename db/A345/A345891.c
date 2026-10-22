/*
 * A345891  a(n) = n + (n - 1) * phi(n).
 * https://oeis.org/A345891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + (n - 1) * phi(n)
 * Range:     u8 n <= 16, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes Euler's totient function for n (parameter type idx_t).
   The result is returned as seq_t. */
static inline seq_t phi_calc(idx_t n)
{
    seq_t phi = (seq_t)n;          /* phi(n) starts as n */
    idx_t m = n;                   /* temp variable for factorisation */

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            /* p is a prime divisor of n */
            while (m % p == 0) {
                m /= p;
            }
            phi -= phi / (seq_t)p;
        }
    }
    if (m > 1) { /* remaining prime factor > sqrt(n) */
        phi -= phi / (seq_t)m;
    }
    return phi;
}

/* Main sequence function as required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                /* behaviour below offset is irrelevant */
    }
    seq_t nseq = (seq_t)n;
    seq_t phi_n = phi_calc(n);
    return nseq + (nseq - 1) * phi_n;
}

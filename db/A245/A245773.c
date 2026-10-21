/*
 * A245773  a(n) = n*sigma(n) - Sum_{(d<n)|n} d*sigma(d).
 * https://oeis.org/A245773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*sigma(n) - Sum_{d|n, d<n} d*sigma(d) * Equivalent: a(n)
 *            = 2*n*sigma(n) - Sum_{d|n} d*sigma(d) (Dirichlet convolution id
 *            * sigma)
 * Range:     u8 n <= 13, u16 n <= 203, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for 0, but return 0 for safety */
    uint64_t m = (uint64_t)n;
    seq_t sigma_n = 1;            /* product of σ(p^e) over prime factors */
    seq_t S_n     = 1;            /* product of ∑_{k=0}^e p^k * σ(p^k) over prime factors */

    /* factor n by trial division */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p != 0) continue;
        int exp = 0;
        while (m % p == 0) { m /= p; ++exp; }

        /* σ(p^exp) = (p^{exp+1}-1)/(p-1) */
        seq_t pow_p = 1;
        for (int i = 0; i < exp; ++i) pow_p *= (seq_t)p;         /* pow_p = p^exp */
        seq_t sigma_factor = ((pow_p * (seq_t)p) - 1) / ((seq_t)p - 1);
        sigma_n *= sigma_factor;

        /* ∑_{k=0}^{exp} p^k * σ(p^k)  computed iteratively */
        seq_t sum_factor = 0;
        seq_t powd = 1;            /* p^k */
        seq_t sigma_k = 1;         /* σ(p^k) */
        for (int k = 0; k <= exp; ++k) {
            sum_factor += powd * sigma_k;
            powd  *= (seq_t)p;                 /* now p^{k+1} */
            sigma_k += powd;                   /* σ(p^{k+1}) = σ(p^k) + p^{k+1} */
        }
        S_n *= sum_factor;
    }

    /* remaining prime factor > sqrt(n), if any */
    if (m > 1) {
        uint64_t p = m;
        int exp = 1;
        seq_t pow_p = (seq_t)p;                    /* p^1 */
        seq_t sigma_factor = ((pow_p * (seq_t)p) - 1) / ((seq_t)p - 1);
        sigma_n *= sigma_factor;

        seq_t sum_factor = 0;
        seq_t powd = 1;            /* p^0 */
        seq_t sigma_k = 1;         /* σ(p^0) */
        for (int k = 0; k <= exp; ++k) {
            sum_factor += powd * sigma_k;
            powd  *= (seq_t)p;
            sigma_k += powd;
        }
        S_n *= sum_factor;
    }

    seq_t result = (seq_t)2 * (seq_t)n * sigma_n - S_n;
    return result;
}

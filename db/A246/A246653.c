/*
 * A246653  G.f.: Sum_{n>=0} x^n / (1-x^2)^(2*n+1) * [Sum_{k=0..n} C(n,k)^2 *
 *          x^k] * [Sum_{k=0..n} C(n,k)^2 * x^(2*k)].
 * https://oeis.org/A246653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(2*k, k) * binomial(n-k,
 *            k)^2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t C2  = 1; /* binomial(0,0) */
    seq_t B   = 1; /* binomial(n,0) */
    idx_t kmax = n / 2;
    for (idx_t k = 0; k <= kmax; ++k) {
        /* term = C(2*k, k) * B^2 */
        seq_t term = C2 * B * B;
        sum += term;

        if (k == kmax) break; /* no more terms */

        /* update C2 to binomial(2*(k+1), k+1) */
        seq_t numC2 = (seq_t)(2*k + 2) * (seq_t)(2*k + 1);
        seq_t denC2 = (seq_t)(k + 1) * (seq_t)(k + 1);
        C2 = C2 * numC2 / denC2;

        /* update B to binomial(n-(k+1), k+1) */
        seq_t numB = (seq_t)(n - 2*k) * (seq_t)(n - 2*k - 1);
        seq_t denB = (seq_t)(n - k) * (seq_t)(k + 1);
        B = B * numB / denB;
    }
    return sum;
}

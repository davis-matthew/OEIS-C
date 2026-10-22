/*
 * A373680  Expansion of e.g.f. exp(x / (1 - x^3)^2).
 * https://oeis.org/A373680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*∑_{k=0..⌊n/3⌋} binomial(2*n-5*k-1, k)/(n-3*k)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Small helper: compute binomial(n,k) for unsigned indices.
   It assumes n >= 0 and works for k > n by returning 0. */
static inline seq_t binom_uint(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;           /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);
        res = res * num;
        res = res / (seq_t)i;           /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Handle the first few terms explicitly (fast path) */
    if (n < 4) {
        /* a(0)=a(1)=a(2)=a(3)=1 */
        return (seq_t)1;
    }

    idx_t kmax = n / 3;
    seq_t sum = (seq_t)0;
    seq_t f = (seq_t)1;      /* falling factorial n!/(n-3k)!    */
    idx_t mprev = n;         /* current value of m = n-3*(k-1)  */

    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t bin;
        if (k == 0) {
            bin = (seq_t)1;
        } else {
            idx_t N = 2ull * n - 5ull * k - 1ull;
            bin = binom_uint(N, k);
        }

        if (k > 0) {
            /* Update f: multiply by (mprev)*(mprev-1)*(mprev-2) */
            f = f * (seq_t)mprev;
            f = f * (seq_t)(mprev - 1);
            f = f * (seq_t)(mprev - 2);
            mprev -= 3;
        }

        sum += bin * f;
    }

    return sum;
}

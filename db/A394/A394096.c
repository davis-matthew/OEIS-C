/*
 * A394096  Number of sets S of nonempty ternary words with n total letters
 *          such that no words in S share letters.
 * https://oeis.org/A394096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 - 9*n - 4)/2 + 3*2^n + 3^n (n > 0), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   mul_small, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute 3·2^n and 3^n */
    seq_t term2 = mul_small(pow2(n), 3);
    seq_t pos    = term2 + pow3(n);

    /* Signed intermediate for the quadratic part */
    long long n_ll = (long long)n;
    long long numer = n_ll * n_ll - 9LL * n_ll - 4LL;
    long long term1 = numer / 2LL;   /* may be negative for small n */

    if (term1 < 0) {
        /* pos – |term1|  */
        seq_t sub = (seq_t)(-term1);
        return pos - sub;
    } else {
        return pos + (seq_t)term1;
    }
}

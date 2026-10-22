/*
 * A332883  If n = Product (p_j^k_j) then a(n) = denominator of Product (1 +
 *          1/p_j^k_j).
 * https://oeis.org/A332883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of ∏_{p^k∥n} (1 + 1/p^k)
 * Range:     u8 n <= 255, u16 n <= 9475, u32 n <= 9475, u64 n <= 9475,
 *            u128 n <= 9475, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Local Euclidean gcd for seq_t values */
static inline seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Return the denominator of the product of unitary reciprocals for n */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 */

    /* Factor n and store each prime power p^k in pow_pe[] */
    idx_t nn = n;
    seq_t pow_pe[20];
    size_t cnt = 0;

    for (idx_t p = 2; p <= nn / p; ++p) {
        if (nn % p == 0) {
            seq_t t = 1;
            while (nn % p == 0) {
                nn /= p;
                t *= (seq_t)p;
            }
            pow_pe[cnt++] = t;            /* p^k */
        }
    }
    if (nn > 1) {                         /* remaining prime factor */
        pow_pe[cnt++] = (seq_t)nn;
    }

    /* Denominator is the product of all prime powers */
    seq_t denom = 1;
    for (size_t i = 0; i < cnt; ++i) {
        denom *= pow_pe[i];
    }

    /* Reduce denom by cancelling common factors with each numerator term
       (p^k + 1) */
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = pow_pe[i] + 1;
        seq_t g = gcd_seq_t(term, denom);
        if (g > 1) {
            denom /= g;
        }
    }
    return denom;
}

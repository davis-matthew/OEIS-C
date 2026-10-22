/*
 * A361099  a(n) = n + 2*binomial(n,2) + 3*binomial(n,3) + 4*binomial(n,4).
 * https://oeis.org/A361099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 2*binomial(n,2) + 3*binomial(n,3) + 4*binomial(n,4)
 * Range:     u8 n <= 6, u16 n <= 25, u32 n <= 401, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* binomial(n,2) = n*(n-1)/2 */
static seq_t binom2(idx_t n)
{
    if (n < 2) return 0;
    seq_t n_seq = (seq_t)n;
    return n_seq * (n_seq - 1) / 2;
}

/* binomial(n,3) = n*(n-1)*(n-2)/6 */
static seq_t binom3(idx_t n)
{
    if (n < 3) return 0;
    seq_t n_seq = (seq_t)n;
    seq_t res = n_seq * (n_seq - 1) / 2;   /* binomial(n,2) */
    return res * (n_seq - 2) / 3;          /* binomial(n,3) */
}

/* binomial(n,4) = n*(n-1)*(n-2)*(n-3)/24 */
static seq_t binom4(idx_t n)
{
    if (n < 4) return 0;
    seq_t n_seq = (seq_t)n;
    seq_t res = n_seq * (n_seq - 1) / 2;   /* binomial(n,2) */
    res = res * (n_seq - 2) / 3;          /* binomial(n,3) */
    return res * (n_seq - 3) / 4;          /* binomial(n,4) */
}

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t res = n_seq;                     /* the n term */

    if (n >= 2) {
        seq_t t = binom2(n);
        res += (seq_t)2 * t;
    }
    if (n >= 3) {
        seq_t t = binom3(n);
        res += (seq_t)3 * t;
    }
    if (n >= 4) {
        seq_t t = binom4(n);
        res += (seq_t)4 * t;
    }
    return res;
}

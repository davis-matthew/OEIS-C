/*
 * A363499  a(n) = Sum_{k=0..n} floor(sqrt(k))^5.
 * https://oeis.org/A363499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 37, u32 n <= 831, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=0..n} floor(sqrt(k))^5
 * Computes the partial sums of the fifth powers of the integer parts of square roots.
 */

#include <stdint.h>

/* integer square root for idx_t (floor of sqrt(n)) */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n, mid, res = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if (mid == 0) {
            res = 0;
            low = 1;
            continue;
        }
        if (mid <= n / mid) {      /* mid*mid <= n, avoid overflow */
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t m = isqrt_idx(n);          /* floor(sqrt(n)) */
    seq_t sum = 0;

    /* sum over full blocks for j = 0 .. m-1 */
    for (idx_t j = 0; j < m; ++j) {
        seq_t j_seq = (seq_t)j;
        seq_t j_sq = j_seq * j_seq;          /* j^2 */
        seq_t j_quad = j_sq * j_sq;          /* j^4 */
        seq_t j_five = j_quad * j_seq;       /* j^5 */
        seq_t term = j_five * ( (seq_t)(2*j + 1) ); /* j^5 * (2j+1) */
        sum += term;
    }

    /* remaining terms for j = m */
    idx_t rem = n - m*m;               /* number of remaining k in block of sqrt = m */
    seq_t m_seq = (seq_t)m;
    seq_t m_sq = m_seq * m_seq;        /* m^2 */
    seq_t m_quad = m_sq * m_sq;        /* m^4 */
    seq_t m_five = m_quad * m_seq;     /* m^5 */
    seq_t add = m_five * ( (seq_t)(rem + 1) ); /* m^5 * (rem+1) */
    sum += add;

    return sum;
}

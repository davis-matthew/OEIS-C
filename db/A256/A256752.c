/*
 * A256752  Number of ways to place non-intersecting diagonals in a convex
 *          (n+2)-gon so as to create no hexagons.
 * https://oeis.org/A256752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1))*∑_{i=0..⌊n/4⌋}∑_{k=i+1..n-3i}
 *            (-1)^i*C(n+k,k)*C(k,i)*C(n-4i-1,k-i-1), if n mod 4 != 0 a(n) =
 *            ((-1)^(n/4)/(n+1))*C(5n/4,n/4) +
 *            (1/(n+1))*∑_{i=0..(n/4)-1}∑_{k=i+1..n-3i}
 *            (-1)^i*C(n+k,k)*C(k,i)*C(n-4i-1,k-i-1), if n mod 4 == 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiplicative binomial coefficient */
static inline seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (n - k + i);
        res = res / i;                  /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* not part of the sequence */

    seq_t denom = (seq_t)(n + 1);
    seq_t pos    = 0;
    seq_t neg    = 0;

    /* Helper macro for adding term with sign */
    #define ADD_TERM(term, sign) \
        do { if (sign > 0) pos += (term); else neg += (term); } while (0)

    if ((n & 3) != 0) /* n mod 4 != 0 */
    {
        idx_t max_i = n / 4;
        for (idx_t i = 0; i <= max_i; ++i)
        {
            int sign = (i & 1) ? -1 : 1;
            idx_t k_start = i + 1;
            idx_t k_max   = n - 3 * i;
            if (k_start > k_max) continue;

            /* initial values for the iterative computation */
            seq_t p1 = binom_idx(n + k_start, k_start);   /* C(n+k, k) */
            seq_t p2 = binom_idx(k_start, i);             /* C(k, i)     */
            seq_t p3 = 1;                                 /* C(N, 0)     */
            idx_t N  = n - 4 * i - 1;                     /* upper of last binomial */
            idx_t m  = 0;                                 /* current lower of C(N, m) */

            for (idx_t k = k_start; k <= k_max; ++k)
            {
                seq_t term = p1 * p2 * p3;
                ADD_TERM(term, sign);

                if (k == k_max) break;   /* avoid overflow on last update */

                /* update p1: C(n+k+1, k+1) */
                p1 = p1 * (n + k + 1) / (k + 1);

                /* update p2: C(k+1, i) */
                p2 = p2 * (k + 1) / ((k + 1) - i);

                /* update p3: C(N, m+1) */
                seq_t mul = (seq_t)(N - m);
                seq_t div = (seq_t)(m + 1);
                p3 = p3 * mul / div;

                ++m;
            }
        }
    }
    else /* n mod 4 == 0 */
    {
        idx_t i4 = n / 4;
        seq_t first = binom_idx((5 * n) / 4, i4);
        ADD_TERM(first, ((i4 & 1) ? -1 : 1));

        idx_t max_i = i4 - 1;
        for (idx_t i = 0; i <= max_i; ++i)
        {
            int sign = (i & 1) ? -1 : 1;
            idx_t k_start = i + 1;
            idx_t k_max   = n - 3 * i;
            if (k_start > k_max) continue;

            seq_t p1 = binom_idx(n + k_start, k_start);
            seq_t p2 = binom_idx(k_start, i);
            seq_t p3 = 1;
            idx_t N  = n - 4 * i - 1;
            idx_t m  = 0;

            for (idx_t k = k_start; k <= k_max; ++k)
            {
                seq_t term = p1 * p2 * p3;
                ADD_TERM(term, sign);

                if (k == k_max) break;

                p1 = p1 * (n + k + 1) / (k + 1);
                p2 = p2 * (k + 1) / ((k + 1) - i);

                seq_t mul = (seq_t)(N - m);
                seq_t div = (seq_t)(m + 1);
                p3 = p3 * mul / div;

                ++m;
            }
        }
    }

    seq_t numerator = pos - neg;          /* guaranteed non‑negative */
    return numerator / denom;
}

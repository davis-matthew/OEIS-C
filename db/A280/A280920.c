/*
 * A280920  Seventh column of Euler's difference table in A068106.
 * https://oeis.org/A280920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n-6} (-1)^j * binomial(n-6,j) * (n-j)! for n >=
 *            7 with a(1)=a(2)=a(3)=a(4)=a(5)=0, a(6)=720
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n <= 5) return (seq_t)0;
    if (n == 6) return factorial(6);

    /* n >= 7 */
    idx_t m = n - 6;          /* number of summation terms minus one */
    seq_t pos = (seq_t)0;     /* sum of terms with even j (positive) */
    seq_t neg = (seq_t)0;     /* sum of terms with odd j (negative) */

    seq_t binom = (seq_t)1;   /* binomial(n-6, j), starts with j = 0 */
    seq_t fact_nj = factorial(n);   /* (n - j)! , starts with j = 0 */

    for (idx_t j = 0; j <= m; ++j) {
        seq_t term = binom * fact_nj;
        if (j & 1)
            neg += term;
        else
            pos += term;

        /* Update for next j, if any */
        if (j < m) {
            /* binom_{j+1} = binom_j * (m - j) / (j + 1) */
            binom = binom * (m - j) / (j + 1);

            /* fact_{n-(j+1)} = fact_{n-j} / (n - j) */
            fact_nj = fact_nj / (n - j);
        }
    }

    /* The final alternating sum is pos - neg (pos >= neg). */
    return pos - neg;
}

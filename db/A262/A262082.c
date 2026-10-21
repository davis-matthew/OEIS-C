/*
 * A262082  Array of coefficients A(n,k) of the formal power series P(n,x)
 *          read by upwards antidiagonals, where P(n,x) = Sum_{k>=0}
 *          A(n,k)*x^k = 1 + x*P(n,x)^(1*n) + x^2*P(n,x)^(2*n) +
 *          x^3*P(n,x)^(3*n) for n >= 0.
 * https://oeis.org/A262082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = 1/(n*k+1) * Σ_{j=0..k} (-2)^j * binomial(n*k+1,j) *
 *            binomial(3*n*k+3-2*j, k-j) a(n) is read from the array A(i,k)
 *            along upwards antidiagonals: index n corresponds to (i,k) with i
 *            = s - k, k = n - s*(s+1)/2, where s = ⌊(√(8n+1)-1)/2⌋. The
 *            function below computes a(n) using the explicit fo...
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 61, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* ------------------------------------------------------------ */
/* Compute A(i,k) from the conjectured closed form              */
/* ------------------------------------------------------------ */
static seq_t A_coeff(idx_t i, idx_t k)
{
    /* trivial cases */
    if (k <= 1)
        return 1;

    /* n*k+1 */
    idx_t nk = i * k;
    idx_t nkt1 = nk + 1;

    seq_t pos_sum = 0;
    seq_t neg_sum = 0;
    seq_t pow2    = 1;          /* 2^j, kept as seq_t */

    for (idx_t j = 0; j <= k; ++j) {
        /* binomial(n*k+1, j)  */
        seq_t term1 = binomial((idx_t)nkt1, j);

        /* binomial(3*n*k+3-2*j, k-j)  */
        idx_t n3k = (idx_t)((seq_t)3 * i * k);
        idx_t arg = n3k + 3 - (idx_t)((seq_t)2 * j);
        seq_t term2 = binomial(arg, k - j);

        seq_t term = term1 * term2;
        term *= pow2;            /* multiply by 2^j */

        if ((j & 1) == 0)
            pos_sum += term;
        else
            neg_sum += term;

        pow2 <<= 1;              /* next power of 2 */
    }

    seq_t diff = pos_sum - neg_sum;          /* non‑negative */
    return diff / (seq_t)nkt1;               /* exact division */
}

/* ------------------------------------------------------------ */
/* Public entry point as required by the test harness          */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Find diagonal s and column index k via incremental search.
       Length of diagonal s is s+1.  cumulative = sum_{t=0}^{s-1}(t+1) = s*(s+1)/2. */
    idx_t s = 0;
    idx_t cumulative = 0;

    while (1) {
        idx_t next_cumulative = cumulative + (s + 1);
        if (n < next_cumulative) {
            break;
        }
        cumulative = next_cumulative;
        ++s;
    }

    idx_t k = n - cumulative;      /* column on diagonal s */
    idx_t i = s - k;                /* row index on that diagonal */

    return A_coeff(i, k);
}

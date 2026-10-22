/*
 * A337996  Triangle read by rows, generalized Eulerian polynomials evaluated
 *          at x = -1.
 * https://oeis.org/A337996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,c) where T(r,c) is the generalized Eulerian
 *            polynomial P(r,c,-1) evaluated at x = -1, with r,c obtained by
 *            flattening the triangle of entries T(r,k). The polynomial is
 *            defined by T(r,c) = (1/2) * Σ_{m=0..r} Σ_{j=0..r+1} (-1)^j *
 *            binomial(r+1,j) * (c*(m-j)+1)^r * signum(c*(m-j)+1) (1) w...
 * Range:     i8 n <= 11, i16 n <= 19, i32 n <= 35, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   binomial, pow_seq_signed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq_signed.h"

/* Compute T(n,k) = P(n,k,-1) according to formula (1). */
static seq_t compute_T(idx_t n, idx_t k)
{
    if (n == 0)
        return 1;                     /* T(0,0) = 1 */

    seq_t sum_alt = 0;

    for (idx_t m = 0; m <= n; ++m) {
        seq_t Sm = 0;
        for (idx_t j = 0; j <= n + 1; ++j) {
            /* (-1)^j */
            int sign_j = (j & 1) ? -1 : 1;

            seq_t bin = binomial(n + 1, j);

            /* Compute base = k*(m-j)+1.  Use signed difference to avoid
               unsigned wrap‑around. */
            long long diff = (long long)m - (long long)j;   /* fits in 64 bits */
            seq_t base = (seq_t)k * (seq_t)diff + 1;

            /* signum(base) */
            int sign_t;
            if (base > 0)
                sign_t = 1;
            else if (base < 0)
                sign_t = -1;
            else
                sign_t = 0;

            if (sign_t == 0)
                continue;   /* term vanishes when base == 0 */

            seq_t pow_val = pow_seq_signed(base, n);

            seq_t term = bin * pow_val;

            if (sign_j < 0)
                term = -term;
            if (sign_t < 0)
                term = -term;

            Sm += term;
        }

        /* Multiply Sm by (-1)^m */
        if ((m & 1) != 0)
            Sm = -Sm;

        sum_alt += Sm;
    }

    return sum_alt / 2;   /* division is exact */
}

/* Public entry point:  a(n)  (flattened indexing). */
seq_t a(idx_t n)
{
    /* Find row r such that sum_{i=0}^{r-1}(i+1) <= n < sum_{i=0}^{r}(i+1). */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t row_start = r * (r + 1) / 2;
    idx_t c = n - row_start;          /* column within the row */

    return compute_T(r, c);
}

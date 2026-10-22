/*
 * A358694  Triangle read by rows. Coefficients of the polynomials H(n, x) =
 *          Sum_{k=0..n-1} Sum_{i=0..k} abs(Stirling1(n, n - i)) * x^(n - k)
 *          in ascending order of powers.
 * https://oeis.org/A358694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^j in H(r,x) where r =
 *            floor((sqrt(8*n+1)-1)/2) and j = n - r*(r+1)/2. H(r,x) =
 *            Σ_{k=0}^{r-1} Σ_{i=0}^{k} |Stirling1(r, r-i)| x^{r-k}.
 *            Equivalently, a(n)=0 for r>0 and j=0, and for 1≤j≤r,
 *            a(n)=Σ_{m=j}^{r} Stirling1(r, m) (unsigned Stirling numbers of
 *            the first kind)
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t N)
{
    /* static state: current row r, its coefficients, and (r)! */
    static seq_t *row = NULL;      /* coefficients of current row r */
    static idx_t current_r  = 0;   /* last computed row index (starts at 0) */
    static seq_t factorial  = 1;   /* factorial of current_r: (current_r)! */

    /* determine the row index r from the overall index N */
    idx_t r;
    if (N == 0) {
        r = 0;
    } else {
        long double t = sqrtl(8.0L * (long double)N + 1.0L);
        r = (idx_t)((t - 1.0L) / 2.0L);
    }

    /* index within the row: j = 0..r */
    idx_t j = N - r * (r + 1) / 2;

    /* extend the precomputation to the needed row if necessary */
    if (current_r < r) {
        for (idx_t n = current_r + 1; n <= r; ++n) {
            idx_t new_len = n + 1;          /* size of the new row (indices 0..n) */
            seq_t *new_row = (seq_t *)malloc(new_len * sizeof(seq_t));
            if (!new_row) {
                abort();                    /* out‑of‑memory: stop */
            }

            /* j = 0 term is always 0 for r > 0 */
            new_row[0] = 0;

            /* j = 1 term */
            if (n == 1) {
                /* factorial currently is (n-1)! = 0! = 1 */
                new_row[1] = factorial;     /* (n-1)*row[1] is 0 for n=1 */
            } else {
                new_row[1] = factorial + (seq_t)(n - 1) * row[1];
            }

            /* j >= 2 terms: use recurrence S(n,j) = S(n-1,j-1) + (n-1)*S(n-1,j) */
            for (idx_t k = 2; k <= n; ++k) {
                seq_t left  = row[k - 1];                     /* S(n-1, k-1) */
                seq_t right = (k <= current_r) ? row[k] : 0;   /* S(n-1, k) or 0 if k>current_r */
                new_row[k]  = left + (seq_t)(n - 1) * right;
            }

            /* free the old row and adopt the new one */
            free(row);
            row = new_row;
            current_r = n;
            factorial *= (seq_t)n;   /* update factorial to n! */
        }
    }

    /* retrieve the requested term */
    if (r == 0) {
        return (seq_t)1;      /* row 0 is just [1] */
    }
    if (j == 0) {
        return (seq_t)0;      /* constant term for n > 0 is always 0 */
    }
    return row[j];
}

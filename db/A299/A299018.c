/*
 * A299018  Triangle read by rows: T(n,k) is the coefficient of x^k in the
 *          polynomial P(n) = n*(x + 1)*P(n - 1) - (n - 2)^2*x*P(n - 2).
 * https://oeis.org/A299018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   The triangular array T(n,k) (coefficients of the polynomials *
 *            P(n)) is defined by * P(0) = 0, P(1) = 1 * P(n) = n*(x + 1)*P(n
 *            - 1) - (n - 2)^2 * x * P(n - 2) * The sequence a(n) is the
 *            flattening (row‑wise) of T(n,k): * a(1)=1, a(2)=2, a(3)=2,
 *            a(4)=6, a(5)=11, a(6)=6, …
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Polynomials: polys[n] points to an array of length n (coefficient of x^k at index k),
 * with polys[0] = NULL (for P(0)=0). */
static seq_t **polys = NULL;            /* pointer array, indexed by n */
static idx_t max_poly_computed = 0;     /* highest n for which polys[n] is ready */

/* Expand the polys array to hold up to 'need' polynomials. */
static void ensure_polys(idx_t need)
{
    if (need <= max_poly_computed)
        return;

    /* Resize the pointer array. */
    polys = (seq_t **)realloc(polys, (need + 1) * sizeof(seq_t *));
    if (!polys)
        abort();                       /* allocation failure */

    /* Initialize polys[0] for P(0)=0. */
    if (max_poly_computed == 0)
        polys[0] = NULL;

    /* Compute subsequent polynomials. */
    for (idx_t n = max_poly_computed + 1; n <= need; ++n)
    {
        polys[n] = (seq_t *)malloc(sizeof(seq_t) * n);
        if (!polys[n])
            abort();                   /* allocation failure */

        if (n == 1)
        {
            polys[1][0] = (seq_t)1;    /* P(1) = 1 */
            continue;
        }

        seq_t *prev     = polys[n - 1];  /* P(n-1), length n-1 */
        seq_t *prevprev = polys[n - 2];  /* P(n-2), length n-2 (NULL when n==2) */

        seq_t m = (seq_t)(n - 2) * (seq_t)(n - 2);  /* (n-2)^2 */

        for (idx_t k = 0; k < n; ++k)
        {
            /* Coefficient of (x+1)*P(n-1) at degree k. */
            seq_t b = 0;
            if (k < n - 1)          /* prev[k] exists when k <= n-2 */
                b += prev[k];
            if (k > 0 && k - 1 < n - 1) /* prev[k-1] exists when k-1 <= n-2 */
                b += prev[k - 1];

            seq_t c = (seq_t)n * b;  /* n * b */

            /* Coefficient coming from (n-2)^2 * x * P(n-2) at degree k. */
            seq_t e = 0;
            if (prevprev != NULL && k > 0 && k - 1 < n - 2) /* prevprev[k-1] exists */
                e = m * prevprev[k - 1];

            polys[n][k] = c - e;
        }
    }
    max_poly_computed = need;
}

/* Return the n‑th term of A299018 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for index 0, guard */

    /* Find the row r that contains the n‑th term. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Ensure all required polynomials are computed. */
    ensure_polys(r);

    /* Number of terms before row r. */
    idx_t offset = r * (r - 1) / 2;
    idx_t k = n - offset - 1;            /* 0‑based index within the row */

    return polys[r][k];
}

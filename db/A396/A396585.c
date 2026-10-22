/*
 * A396585  Triangle read by rows: the Motzkin number square. T(n, k) = T(n,
 *          k+1) - T(n-1, k) with initial values T(n, n) = A001006(n)^2.
 * https://oeis.org/A396585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where T(r,c) = Sum_{j=0..r-c} (-1)^j * *
 *            binomial(r-c,j) * A001006(r-j)^2. * The triangle is read row by
 *            row: a(0)=1, a(1)=0, a(2)=1, a(3)=3,
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 111, u64 n <= 338,
 *            u128 n <= 1114, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *M = NULL;        /* array of Motzkin numbers          */
static idx_t M_cap = 0;        /* currently allocated capacity      */
static idx_t M_len = 0;        /* number of computed Motzkin entries */

/* Compute the nth term of A396585 */
seq_t a(idx_t n)
{
    /* Find the row r and column c such that
     * n = r*(r+1)/2 + c   with 0 <= c <= r.   */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t c = n - r * (r + 1) / 2;

    /* Ensure Motzkin numbers M[0..r] are available.  */
    if (M_cap < r + 1) {
        idx_t new_cap = r + 5;                 /* spare slots */
        seq_t *tmp = (seq_t *)realloc(M, new_cap * sizeof(seq_t));
        M = tmp;
        M_cap = new_cap;
    }
    for (idx_t i = M_len; i <= r; ++i) {
        if (i == 0) {
            M[0] = (seq_t)1;
        } else if (i == 1) {
            M[1] = (seq_t)1;
        } else {
            /* M(i) = ((2*i+1)*M(i-1) + (3*i-3)*M(i-2)) / (i+2) */
            seq_t num1 = ((seq_t)(2 * (seq_t)i + 1)) * M[i - 1];
            seq_t num2 = ((seq_t)(3 * (seq_t)i - 3)) * M[i - 2];
            seq_t num = num1 + num2;
            seq_t den = (seq_t)(i + 2);
            M[i] = num / den;
        }
    }
    M_len = r + 1;

    /* Compute T(r,c) by the binomial sum. */
    seq_t result = 0;
    idx_t maxj = r - c;                     /* n-k in the OEIS formula   */
    for (idx_t j = 0; j <= maxj; ++j) {
        seq_t term = binomial(maxj, j);
        term = term * M[r - j];
        term = term * M[r - j];
        if (j & 1U)
            result -= term;
        else
            result += term;
    }

    return result;
}

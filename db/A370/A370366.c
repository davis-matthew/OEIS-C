/*
 * A370366  Number A(n,k) of partitions of [k*n] into n sets of size k having
 *          no set of consecutive numbers whose maximum (if k>0) is a multiple
 *          of k; square array A(n,k), n>=0, k>=0, read by antidiagonals.
 * https://oeis.org/A370366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = sum_{j=0}^n (-1)^j * C(n,j) * P(n-j,k), where P(m,k) =
 *            (m*k)! / ((k!)^m * m!). * P(m,k) is the number of partitions of
 *            a set of size m*k * into m blocks of size k (unlabeled)
 * Range:     u8 n <= 22, u16 n <= 31, u32 n <= 48, u64 n <= 82, u128 n <= 126,
 *            big any n
 * Helpers:   antidiagonal_coords, binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/antidiagonal_coords.h"
#include "../utils/factorial.h"
#include "../utils/binomial.h"

/* Compute P(m,k) = (m*k)! / ((k!)^m * m!) */
static seq_t partitions_of_size(idx_t m, idx_t k)
{
    if (m == 0)      /* empty set: one partition */
        return (seq_t)1;
    seq_t prod = (seq_t)1;
    for (idx_t r = 1; r <= m; ++r) {
        prod = prod * binomial(r * k, k);
    }
    return prod / factorial(m);
}

/* Main sequence accessor */
seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);      /* i = number of sets (n), j = size (k) */

    /* Handle the trivial cases with k = 0 or n = 0 */
    if (j == 0) {
        if (i == 0)
            return (seq_t)1;              /* A(0,0) = 1 */
        else
            return (seq_t)0;              /* A(n>0,0) = 0 */
    }
    if (i == 0)
        return (seq_t)1;                  /* A(0,k) = 1 for any k > 0 */

    /* Inclusion–exclusion: sum_{t=0}^i (-1)^t * C(i,t) * P(i-t,j) */
    seq_t sum_even = (seq_t)0;
    seq_t sum_odd  = (seq_t)0;

    for (idx_t t = 0; t <= i; ++t) {
        seq_t comb = binomial(i, t);                 /* C(i,t) */
        seq_t part = partitions_of_size(i - t, j);   /* P(i-t,j) */

        if ((t & 1) == 0)  /* even t */
            sum_even = sum_even + comb * part;
        else
            sum_odd  = sum_odd  + comb * part;
    }

    /* The result is non‑negative; subtracting the smaller sum is safe. */
    return sum_even - sum_odd;
}

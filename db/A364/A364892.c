/*
 * A364892  Row sums of A364891.
 * https://oeis.org/A364892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} ((-1)^(k-1) * Sum_{j=0..k-1} (-1)^j * (p(n -
 *            j*(2*j + 1)) - p(n - (j + 1)*(2*j + 1)))) where p(m) is the
 *            partition number A000041(m)
 * Range:     u8 n <= 21, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of partition numbers */
static seq_t *partitions = NULL;
static idx_t max_computed = 0;      /* highest index +1 for which partitions is computed  */

/* Compute partition numbers up to n using Euler's pentagonal number recurrence.
   Only (void*) to seq_t* conversion is cast to satisfy C++ */
static void ensure_partitions(idx_t n)
{
    if (n + 1 <= max_computed)
        return;                       /* already computed up to n */

    /* Allocate or grow array */
    if (partitions == NULL || max_computed < n + 1) {
        seq_t *newp = (seq_t *)realloc(partitions, (n + 1) * sizeof(seq_t));
        if (!newp)
            return;                  /* allocation failed – will return zero later */
        partitions = newp;
    }

    /* Initialize p[0] */
    if (max_computed == 0) {
        partitions[0] = (seq_t)1;
        max_computed = 1;
    }

    /* Compute from the first missing index up to n */
    for (idx_t m = max_computed; m <= n; ++m) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > m)
                break;
            seq_t term1 = partitions[m - pent1];
            if (k & 1)
                pos += term1;
            else
                neg += term1;
            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 > m)
                continue;
            seq_t term2 = partitions[m - pent2];
            if (k & 1)
                pos += term2;
            else
                neg += term2;
        }
        partitions[m] = pos - neg;   /* correct signed result */
    }

    max_computed = n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    ensure_partitions(n);

    seq_t ans = 0;
    idx_t n_parity = n & 1;    /* n mod 2 */

    for (idx_t j = 0; ; ++j) {
        idx_t s = j * (2 * j + 1);
        if (s > n)
            break;

        if ( ((j & 1) ^ n_parity) ) {   /* j and n have opposite parity   */
            idx_t t = (j + 1) * (2 * j + 1);
            seq_t term = partitions[n - s];
            if (t <= n)
                term -= partitions[n - t];
            ans += term;
        }
    }

    return ans;
}

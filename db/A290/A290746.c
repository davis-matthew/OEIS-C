/*
 * A290746  Total number of distinct Lyndon factors appearing in all words of
 *          length n over an alphabet of size 2.
 * https://oeis.org/A290746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} (n−k+1)·2^{n−k}·L₂(k) , where L₂(k) = number
 *            of binary Lyndon words of length k. L₂(k) is computed
 *            recursively by L₂(k) = (2^k − Σ_{d|k,d<k} d·L₂(d)) / k
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 56, u128 n <= 100,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

/* static storage for Lyndon counts up to the largest n seen */
static idx_t max_prepared = 0;
static seq_t *Loly = NULL;   /* L₂(k) values */

/* ensure that Loly[1..n] are computed */
static void prepare_lyndon(idx_t n)
{
    if (n <= max_prepared) return;

    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(Loly, new_size * sizeof(seq_t));
    if (!tmp) exit(1);                 /* memory failure */
    Loly = tmp;
    if (max_prepared == 0) Loly[0] = 0; /* unused entry */

    for (idx_t i = max_prepared + 1; i <= n; ++i) {
        seq_t pow = pow2(i);        /* 2^i */
        seq_t sum = pow;

        /* subtract contributions of proper divisors d */
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                idx_t d1 = d;
                idx_t d2 = i / d;
                if (d1 < i)
                    sum -= Loly[d1] * (seq_t)d1;
                if (d2 != d1 && d2 < i)
                    sum -= Loly[d2] * (seq_t)d2;
            }
        }
        Loly[i] = sum / (seq_t)i;   /* L₂(i) */
    }
    max_prepared = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    prepare_lyndon(n);

    seq_t total = 0;
    seq_t pow = pow2(n - 1);                 /* 2^{n-1} for k=1 */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = Loly[k] * (seq_t)(n - k + 1) * pow;
        total += term;
        pow /= (seq_t)2;                     /* next exponent */
    }
    return total;
}

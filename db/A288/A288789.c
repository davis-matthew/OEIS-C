/*
 * A288789  Number of blocks of size >= seven in all set partitions of n.
 * https://oeis.org/A288789
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n-7} binomial(n,j) * Bell(j) where Bell(0)=1
 *            and Bell(m+1)=Sum_{k=0..m} binomial(m,k)*Bell(k) The function
 *            computes a(n) for n ≥ 7 using memoised Bell numbers. It supports
 *            any seq_t and idx_t that the build system supplies
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 17, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *bell = NULL;   /* Bell numbers B(0), B(1), ... */
static idx_t bell_len = 0;   /* number of computed Bell numbers */

/* Ensure Bell numbers are computed up to index n.  The array is
   expanded if necessary. */
static void
ensure_bell(idx_t n)
{
    if (n < bell_len)
        return;

    size_t new_len = (size_t)(n + 1);
    seq_t *new_bell = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (new_bell == NULL)
        exit(1);               /* allocation failed */

    /* copy existing values */
    if (bell_len > 0)
        memcpy(new_bell, bell, bell_len * sizeof(seq_t));
    if (bell)
        free(bell);
    bell = new_bell;

    /* compute missing Bell numbers */
    for (idx_t m = bell_len; m <= n; ++m) {
        if (m == 0) {
            bell[0] = (seq_t)1;
            continue;
        }

        seq_t sum = 0;
        seq_t bin = 1;                 /* binomial(m-1,0) */
        for (idx_t k = 0; k < m; ++k) {
            sum += bin * bell[k];
            if (k + 1 < m)             /* update to binomial(m-1,k+1) */
                bin = bin * ((seq_t)(m - 1 - k)) / ((seq_t)(k + 1));
        }
        bell[m] = sum;
    }
    bell_len = new_len;
}

seq_t
a(idx_t n)
{
    if (n < 7)
        return (seq_t)0;          /* values below the offset are irrelevant */

    ensure_bell(n);              /* compute Bell numbers up to n */

    idx_t limit = n - 7;
    seq_t res = 0;
    seq_t binom = 1;             /* binomial(n,0) */

    for (idx_t j = 0; j <= limit; ++j) {
        if (j > 0)
            binom = binom * ((seq_t)(n - j + 1)) / ((seq_t)j);
        res += binom * bell[j];
    }
    return res;
}

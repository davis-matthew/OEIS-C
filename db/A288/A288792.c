/*
 * A288792  Number of blocks of size >= ten in all set partitions of n.
 * https://oeis.org/A288792
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n-10} binomial(n,j) * Bell(j)
 * Range:     u8 n <= 12, u16 n <= 14, u32 n <= 19, u64 n <= 28, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of blocks of size >= 10 in all set partitions of n.   */
/* Implements memoised computation of Bell numbers up to the largest n-10 */
/* encountered. The routine grows a static array of Bell numbers lazily.   */
#include <stdlib.h>

static seq_t *bell = NULL;       /* array of Bell numbers */
static idx_t bell_count = 0;     /* number of Bell entries stored (index+1) */

/* Ensure Bell numbers up to index 'upto' are computed and stored. */
static void ensure_bell(idx_t upto)
{
    if (upto + 1 <= bell_count) return;           /* already have enough */

    /* Extend the array */
    seq_t *tmp = (seq_t *)realloc(bell, (upto + 1) * sizeof(seq_t));
    if (!tmp) exit(1);                            /* allocation failed */
    bell = tmp;

    /* Compute any new Bell numbers */
    for (idx_t k = bell_count; k <= upto; ++k) {
        if (k == 0) {                            /* base case B_0 = 1 */
            bell[0] = 1;
            continue;
        }
        seq_t sum = 0;
        seq_t bin = 1;                           /* current binomial coefficient binom(k-1,i) */
        for (idx_t i = 0; i < k; ++i) {
            sum += bin * bell[i];
            /* update binomial: binom(k-1,i+1) = binom(k-1,i) * (k-1-i)/(i+1) */
            bin = bin * (seq_t)(k - 1 - i) / (i + 1);
        }
        bell[k] = sum;
    }
    bell_count = upto + 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 10) return 0;                       /* offset sentinel   */

    idx_t m = n - 10;                            /* number of terms to sum */
    ensure_bell(m);                              /* guarantee Bell[0..m] exist */

    seq_t sum = 0;
    seq_t bin = 1;                               /* binom(n,0) = 1 */
    for (idx_t j = 0; j <= m; ++j) {
        sum += bin * bell[j];
        if (j < m) {                             /* update to binom(n,j+1) */
            bin = bin * (seq_t)(n - j) / (j + 1);
        }
    }
    return sum;
}

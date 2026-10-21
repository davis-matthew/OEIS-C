/*
 * A286896  Number of blocks of size >= n in all set partitions of [2n].
 * https://oeis.org/A286896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} binomial(2*n,j) * Bell(j)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where Bell(0)=1 and Bell(m+1)=sum_{k=0..m} binomial(m,k)*Bell(k)   */
#include <stdlib.h>

static seq_t *bell = NULL;
static idx_t bell_len = 0;

/* Ensure that Bell numbers up to n are stored in the global array */
static void ensure_bell(idx_t n)
{
    if (bell_len == 0) {
        bell = (seq_t *)malloc(sizeof(seq_t));
        if (!bell) exit(1);
        bell[0] = 1;
        bell_len = 1;
    }
    while (bell_len <= n) {
        idx_t m = bell_len;          /* we are computing Bell(m) */
        seq_t sum  = 0;
        seq_t binom = 1;             /* C(m-1,0) */
        for (idx_t k = 0; k < m; ++k) {
            sum += bell[k] * binom;
            if (k == m - 1) break;
            binom = (binom * ((m - 1) - k)) / (k + 1);
        }
        seq_t *tmp = (seq_t *)realloc(bell, (bell_len + 1) * sizeof(seq_t));
        if (!tmp) exit(1);
        bell = tmp;
        bell[bell_len] = sum;
        ++bell_len;
    }
}

seq_t a(idx_t n)
{
    ensure_bell(n);                 /* need Bell[0..n] */
    seq_t result = 0;
    seq_t binom = 1;                /* C(2*n,0) */
    seq_t two_n = (seq_t)(2 * n);    /* 2*n as seq_t */

    for (idx_t j = 0; j <= n; ++j) {
        result += bell[j] * binom;
        if (j == n) break;
        binom = (binom * ((seq_t)(two_n - j))) / (j + 1);
    }
    return result;
}

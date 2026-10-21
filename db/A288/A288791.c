/*
 * A288791  Number of blocks of size >= nine in all set partitions of n.
 * https://oeis.org/A288791
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Bell(n+1) - Sum_{j=0..8} binomial(n,j) * Bell(n-j)
 * Range:     u8 n <= 10, u16 n <= 13, u32 n <= 18, u64 n <= 27, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                                         (using the recurrence          */
/*                                          B_{m+1} = Sum_{k=0}^m C(m,k)B_k   */

#include <stddef.h>            /* for NULL */

#define MAX_N 12000            /* sufficient for n up to about 10000 */

/* caches for Bell numbers (Bell[0] .. Bell[MAX_N+1]) and the sequence a(n) */
static seq_t Bell[MAX_N + 2] = {0};     /* Bell[0] will be set to 1 on first use  */
static seq_t A_cache[MAX_N + 1] = {0};  /* 0 value indicates “not yet computed”   */

static idx_t bell_last_computed = 0;    /* last index m for which Bell[m] is known  */

/* make sure Bell numbers up to index m (inclusive) are computed */
static void ensure_bell_up_to(idx_t m)
{
    if (bell_last_computed >= m) return;            /* nothing to do */

    if (bell_last_computed == 0) {                  /* initialise B[0] */
        Bell[0] = (seq_t)1;
    }

    for (idx_t target = bell_last_computed + 1; target <= m; ++target) {
        /* compute Bell[target] using the recurrence with m = target-1 */
        seq_t sum = 0;
        seq_t binom = 1;                            /* C(target-1,0) */
        idx_t upper = target - 1;                   /* last k in the inner sum */

        for (idx_t k = 0; k <= upper; ++k) {
            sum += binom * Bell[k];
            if (k < upper) {
                seq_t mult = (seq_t)(upper - k);
                seq_t denom = (seq_t)(k + 1);
                binom = (binom * mult) / denom;
            }
        }
        Bell[target] = sum;
    }
    bell_last_computed = m;
}

/* external linkage required by the specification   */
seq_t a(idx_t n)
{
    /* According to the OEIS offset, values below 9 are undefined; return 0. */
    if (n < 9) return (seq_t)0;

    /* We need Bell[n+1] and Bell[n-j] for j=0..8. */
    ensure_bell_up_to(n + 1);

    /* Use memoisation – all a(n) for n>=9 are positive   */
    if (A_cache[n] == (seq_t)0) {
        seq_t res = Bell[n + 1];          /* B_{n+1} */
        seq_t binom = 1;                  /* C(n,0)      */

        for (idx_t j = 0; j <= 8; ++j) {
            res -= binom * Bell[n - j];
            if (j < 8) {
                seq_t num = (seq_t)(n - j);
                seq_t den = (seq_t)(j + 1);
                binom = (binom * num) / den;
            }
        }
        A_cache[n] = res;
    }
    return A_cache[n];
}

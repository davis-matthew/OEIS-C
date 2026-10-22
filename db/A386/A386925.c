/*
 * A386925  a(n) = numerator(Sum_{k=1..n} d(k+1)/d(k)), where d is the number
 *          of divisors function.
 * https://oeis.org/A386925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator( Σ_{k=1..n} d(k+1)/d(k) ) where d(m) is the
 *            number of divisors of m. The function keeps a divisor‑count
 *            table up to the largest index seen and recomputes it when a
 *            larger n is requested. Complexity: O(n log n) per new maximum n
 * Range:     u8 n <= 8, u16 n <= 37, u32 n <= 1331, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *div_cnt = NULL;   /* divisor counts: d(0)=0, d(1)=1 … */
static idx_t  limit  = 0;       /* highest m for which div_cnt is valid */

/* Recompute divisor counts for all m in [0..m_max].  The array is
   reallocated if necessary.  All existing data are discarded on
   recomputation.  */
static void
ensure_divisor_counts(idx_t m_max)
{
    if (m_max <= limit && div_cnt != NULL)
        return;                         /* already up to date */

    /* Allocate or grow the array */
    seq_t *new_tbl = (seq_t *)realloc(div_cnt, (m_max + 1) * sizeof(seq_t));
    if (!new_tbl) {
        /* Allocation failed – abort the program.  */
        exit(EXIT_FAILURE);
    }
    div_cnt = new_tbl;

    /* Initialize all entries to zero */
    for (idx_t i = 0; i <= m_max; ++i)
        div_cnt[i] = (seq_t)0;

    /* Count divisors */
    for (idx_t i = 1; i <= m_max; ++i) {
        for (idx_t j = i; j <= m_max; j += i) {
            div_cnt[j] += (seq_t)1;
        }
    }

    limit = m_max;
}

/* Return the numerator of the reduced sum Σ_{k=1..n} d(k+1)/d(k).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for n=0, but harmless */

    /* We need divisor counts up to n+1 (for d(n+1)).  */
    ensure_divisor_counts(n + 1);

    seq_t num = (seq_t)0;  /* numerator of the current sum   */
    seq_t den = (seq_t)1;  /* denominator of the current sum   */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t ak = div_cnt[k + 1];   /* d(k+1) */
        seq_t bk = div_cnt[k];       /* d(k)   */

        /* new_num/ new_den = num/den + ak/bk          */
        /* new_num = num*bk + den*ak                   */
        /* new_den = den*bk                            */
        seq_t new_num = num * bk + den * ak;
        seq_t new_den = den * bk;

        /* Reduce the fraction.  (gcd is defined for seq_t in utils/gcd.h) */
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    return num;   /* numerator of the fully reduced sum */
}

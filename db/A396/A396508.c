/*
 * A396508  Number of palindromic special Motzkin paths of weight n.
 * https://oeis.org/A396508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 27, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence implementation of OEIS A396508
   a(2*m)   = b(m)
   a(2*m+1) = 1 + 3*b(m) + 4*∑_{i=1}^{m-1} b(i)

   b(0)=0, b(1)=1, b(2)=3
   for m ≥ 2: (m+1)*b(m+1) = (5*m+1)*b(m) + (m+7)*b(m-1) - 5*(m-1)*b(m-2)
*/

#include <stdlib.h>
#include <stddef.h>

static seq_t *b_cache   = NULL;   /* b(m) values, indexed from 0 */
static seq_t *sum_cache = NULL;   /* prefix sums sum_b[m] = Σ_{i=1}^m b(i) */
static size_t b_cap    = 0;        /* allocated capacity of the arrays   */
static idx_t  b_max    = 0;        /* largest index currently computed   */

/* Ensure arrays are large enough and b(m) is computed up to m_needed */
static void ensure_b_up_to(idx_t m_needed)
{
    /* First‑time initialisation */
    if (b_cache == NULL) {
        size_t init_cap = 8;
        b_cache   = (seq_t *)malloc(init_cap * sizeof(seq_t));
        sum_cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (b_cache == NULL || sum_cache == NULL) {
            /* Allocation failure: terminate the program */
            exit(1);
        }

        /* Base values */
        b_cache[0]   = (seq_t)0;
        sum_cache[0] = (seq_t)0;
        if (init_cap > 1) {
            b_cache[1]   = (seq_t)1;
            sum_cache[1] = (seq_t)1;
        }
        if (init_cap > 2) {
            b_cache[2]   = (seq_t)3;
            sum_cache[2] = sum_cache[1] + b_cache[2];   /* 1 + 3 = 4 */
        }

        b_max = 2;
        b_cap = init_cap;
    }

    /* Nothing to do if already up to date */
    if (m_needed <= b_max) return;

    /* Resize arrays if necessary */
    size_t needed = (size_t)(m_needed + 1);
    if (b_cap < needed) {
        size_t new_cap = b_cap ? b_cap : 8;
        while (new_cap < needed) new_cap <<= 1;
        b_cache   = (seq_t *)realloc(b_cache,   new_cap * sizeof(seq_t));
        sum_cache = (seq_t *)realloc(sum_cache, new_cap * sizeof(seq_t));
        if (b_cache == NULL || sum_cache == NULL) exit(1);
        b_cap = new_cap;
    }

    /* Compute new values using the recurrence */
    for (idx_t m = b_max; m < m_needed; ++m) {
        /* (m+1)*b(m+1) = (5m+1)*b(m) + (m+7)*b(m-1) - 5*(m-1)*b(m-2) */
        seq_t term1 = (seq_t)((5 * m + 1)) * b_cache[m];
        term1      += (seq_t)(m + 7) * b_cache[m - 1];

        seq_t term2 = (seq_t)5 * (m - 1) * b_cache[m - 2];

        seq_t numerator = term1 - term2;
        seq_t new_b     = numerator / (seq_t)(m + 1);

        b_cache[m + 1]   = new_b;
        sum_cache[m + 1] = sum_cache[m] + new_b;
    }
    b_max = m_needed;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* defined behaviour for non‑positive n is arbitrary */

    if ((n & 1) == 0) {           /* even index: a(2*m) = b(m) */
        idx_t m = n >> 1;
        ensure_b_up_to(m);
        return b_cache[m];
    } else {                      /* odd index: a(2*m+1) = 1 + 3*b(m) + 4*∑_{i=1}^{m-1}b(i) */
        idx_t m = (n - 1) >> 1;   /* floor((n-1)/2) */
        ensure_b_up_to(m);
        seq_t sum = (m >= 2) ? sum_cache[m - 1] : (seq_t)0;
        return (seq_t)1 + (seq_t)3 * b_cache[m] + (seq_t)4 * sum;
    }
}

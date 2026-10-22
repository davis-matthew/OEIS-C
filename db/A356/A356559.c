/*
 * A356559  a(n) = exp(-1) * n! * Sum_{k>=0} Laguerre(n,k) / k!.
 * https://oeis.org/A356559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(n,k)^2 * k! *
 *            Bell(n-k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A356559) */
#include <stdlib.h>

/* External integer types supplied by the build */
extern seq_t *seq_alloc(size_t);
extern void seq_free(seq_t *);

static seq_t *fact = NULL;          /* factorials up to current max */
static seq_t *bell = NULL;          /* Bell numbers up to current max */
static idx_t fact_cap = 0;
static idx_t bell_cap = 0;

/* Utility: grow the factorial and Bell arrays up to size 'need' */
static void ensure_arrays(idx_t need)
{
    /* First allocation */
    if (!fact) {
        /* allocate arrays for up to (need) elements; we overallocate a bit */
        size_t alloc_size = (size_t)(need + 41);  /* small slack */
        fact = (seq_t*)malloc(alloc_size * sizeof(seq_t));
        bell = (seq_t*)malloc(alloc_size * sizeof(seq_t));
        if (!fact || !bell) {
            /* Out of memory – abort; caller is expected to handle */
            exit(1);
        }
        fact[0] = 1;
        bell[0] = 1;
        fact_cap = 0;
        bell_cap = 0;
    }

    /* Extend factorials if needed */
    if (fact_cap < need) {
        for (idx_t i = fact_cap + 1; i <= need; ++i)
            fact[i] = fact[i-1] * i;
        fact_cap = need;
    }

    /* Extend Bell numbers if needed */
    if (bell_cap < need) {
        for (idx_t m = bell_cap + 1; m <= need; ++m) {
            seq_t sum = 0;
            seq_t binom = 1;                 /* C(m-1,0)=1 */
            idx_t upper = m - 1;
            for (idx_t k = 0; k <= upper; ++k) {
                sum += binom * bell[k];
                if (k < upper) {
                    binom = binom * (upper - k) / (k + 1);
                }
            }
            bell[m] = sum;
        }
        bell_cap = need;
    }
}

seq_t a(idx_t n)
{
    ensure_arrays(n);

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;
    seq_t binom = 1;     /* binomial(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = binom * binom;          /* (n choose k)^2 */
        term = term * fact[k];               /* * k! */
        term = term * bell[n - k];           /* * Bell(n-k) */

        if ((n - k) & 1U)          /* sign = (-1)^(n-k) */
            sum_neg += term;
        else
            sum_pos += term;

        /* Update binomial for next k */
        if (k < n) {
            binom = binom * (n - k) / (k + 1);
        }
    }

    return sum_pos - sum_neg;   /* final result is non‑negative */
}

/*
 * A375662  Expansion of e.g.f. 1 / (1 - x^2 * (exp(x) - 1))^2.
 * https://oeis.org/A375662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (k+1)! *
 *            Stirling2(n-2k,k)/(n-2k)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via the recurrence:
   b(0) = 1, b(1)=b(2)=0,
   b(n) = Σ_{k=3..n} C(n,k) * k*(k-1) * b(n-k),
   a(n) = Σ_{i=0..n} C(n,i) * b(i) * b(n-i). */

#include <stdlib.h>

static seq_t *b = NULL;          /* array of b(n) values */
static idx_t  b_cap = 0;         /* size of allocated array */

/* Ensure that b[0..n] are computed. */
static void ensure_b(idx_t n)
{
    if (n < b_cap)
        return;

    /* Allocate or grow the array. */
    size_t new_cap = (size_t)(n + 1);
    if (b == NULL) {
        b = (seq_t *)malloc(sizeof(seq_t) * new_cap);
    } else {
        b = (seq_t *)realloc(b, sizeof(seq_t) * new_cap);
    }
    if (b == NULL) {
        /* allocation failure – terminate */
        abort();
    }

    /* Compute new elements of b. */
    for (idx_t i = b_cap; i <= n; ++i) {
        if (i == 0) {
            b[0] = (seq_t)1;
            continue;
        }
        if (i == 1 || i == 2) {
            b[i] = (seq_t)0;
            continue;
        }

        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;                 /* binom(i,0) */
        for (idx_t k = 1; k <= i; ++k) {
            /* Update binomial coefficient binom(i,k) */
            binom = binom * (seq_t)(i - k + 1); /* multiply by (i-k+1) */
            binom = binom / (seq_t)k;          /* divide by k  (exact) */

            if (k >= 3) {
                seq_t ck = (seq_t)k * (seq_t)(k - 1);   /* k*(k-1)   */
                seq_t term = binom;
                term = term * ck;                      /* binom(k) * k*(k-1) */
                term = term * b[i - k];
                sum = sum + term;
            }
        }
        b[i] = sum;
    }

    b_cap = new_cap;
}

/* Compute the sequence value a(n). */
seq_t a(idx_t n)
{
    ensure_b(n);

    seq_t res = (seq_t)0;
    seq_t binom = (seq_t)1;     /* binom(n,0) */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t term = binom;
        term = term * b[i];
        term = term * b[n - i];
        res = res + term;

        if (i < n) {              /* prepare next binomial coefficient */
            binom = binom * (seq_t)(n - i);
            binom = binom / (seq_t)(i + 1);   /* exact division */
        }
    }

    return res;
}

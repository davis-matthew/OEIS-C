/*
 * A379674  Expansion of e.g.f. -log( 1 - log(1 - x)^4 / 24 ).
 * https://oeis.org/A379674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A379674: Expansion of e.g.f. -log( 1 - log(1 - x)^4 / 24 ).
   Formula (OEIS): a(n) = Σ_{k=1..⌊n/4⌋} (4*k)! * |s(n,4*k)|/(k*24^k),
   where s(n,m) is the Stirling number of the first kind.
   Recurrence used: a(0)=a(1)=a(2)=a(3)=0, a(4)=1,
   a(n)=|s(n,4)| + Σ_{k=1}^{n-1} |s(k,4)|*C(n-1,k)*a(n-k). */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Called from the build system.  The types seq_t and idx_t are defined
   elsewhere.  No other helpers are needed. */
seq_t a(idx_t n)
{
    /* Single static cache of previously computed values. */
    static seq_t *a_vals = NULL;      /* a(k) for 0..max_n */
    static seq_t *s4_vals = NULL;     /* |s(k,4)| for 0..max_n */
    static size_t max_n = 0;           /* highest n already computed */
    /* For unsigned Stirling numbers of the first kind, keep the row
       for the current n-1:  c(k)=c(n-1,k) for k=0..4. */
    static seq_t c_prev[5] = {1,0,0,0,0};

    /* Ensure enough space for n and for index 4 (base case). */
    size_t need_cap = (size_t)(n + 1);
    if (need_cap < 5) need_cap = 5;    /* to hold a(4) and s4(4) */

    if (max_n == 0) {
        /* First access: allocate arrays with required capacity. */
        a_vals   = (seq_t *)realloc(a_vals,   need_cap * sizeof(seq_t));
        s4_vals  = (seq_t *)realloc(s4_vals,  need_cap * sizeof(seq_t));
        if (!a_vals || !s4_vals) exit(EXIT_FAILURE);

        /* Initialise base indices 0..4. */
        a_vals[0] = a_vals[1] = a_vals[2] = a_vals[3] = 0;
        a_vals[4] = 1;          /* a(4)=1 */
        s4_vals[0] = s4_vals[1] = s4_vals[2] = s4_vals[3] = 0;
        /* s(4,4)=1 for unsigned Stirling numbers */
        s4_vals[4] = 1;

        /* c_prev already holds the row for n=0: [1,0,0,0,0]. */
    } else if (need_cap > max_n + 1) {
        /* Grow the arrays if necessary. */
        seq_t *new_a   = (seq_t *)realloc(a_vals,   need_cap * sizeof(seq_t));
        seq_t *new_s4  = (seq_t *)realloc(s4_vals,  need_cap * sizeof(seq_t));
        if (!new_a || !new_s4) exit(EXIT_FAILURE);
        a_vals = new_a;
        s4_vals = new_s4;
    }

    /* If we already have the requested value, return it immediately. */
    if (n <= max_n) return a_vals[n];

    /* Compute values for indices max_n+1 .. n.  The algorithm
       increments n step by step, keeping the previous Stirling row
       in c_prev. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* Compute unsigned Stirling numbers of the first kind for i. */
        seq_t c_cur[5] = {0};
        c_cur[0] = 0;
        for (int k = 1; k <= 4; ++k) {
            seq_t term1 = (k - 1 <= (int)i - 1) ? c_prev[k - 1] : 0;
            seq_t term2 = (idx_t)(i - 1) * ((k <= (int)i - 1) ? c_prev[k] : 0);
            c_cur[k] = term1 + term2;
        }
        /* Store |s(i,4)| for later use. */
        s4_vals[i] = c_cur[4];

        /* Recurrence for a(i).  Base cases handled explicitly. */
        if (i <= 3) {
            a_vals[i] = 0;
        } else if (i == 4) {
            a_vals[i] = 1;
        } else {
            seq_t sum = 0;
            seq_t binom = 1;          /* C(i-1,0) = 1 */
            for (idx_t k = 1; k <= i - 1; ++k) {
                /* Update binomial coefficient: C(i-1,k) = C(i-1,k-1)*(i-k)/k */
                binom = (binom * (seq_t)(i - k)) / (seq_t)k;
                if (s4_vals[k] != 0) {
                    seq_t term = s4_vals[k];
                    term = term * binom;
                    term = term * a_vals[i - k];
                    sum  = sum + term;
                }
            }
            a_vals[i] = s4_vals[i] + sum;
        }

        /* Move current Stirling row to previous for next iteration. */
        for (int k = 0; k <= 4; ++k) c_prev[k] = c_cur[k];
    }

    max_n = (size_t)n;
    return a_vals[n];
}

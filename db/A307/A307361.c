/*
 * A307361  Expansion of e.g.f. (sinh(x) + 5*cosh(x) - 5)/(3 - 2*cosh(x)).
 * https://oeis.org/A307361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(n-k) * Stirling2(n,k) * k! * Lucas(k)
 *            Lucas(0)=2, Lucas(1)=1, Lucas(n)=Lucas(n-1)+Lucas(n-2)
 *            Stirling2(n,k) satisfies: S(n,k)=S(n-1,k-1)+k*S(n-1,k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t max_n = 0;
static seq_t *a_val = NULL;
static seq_t *lucas = NULL;
static seq_t *fact  = NULL;
static seq_t *b     = NULL;
static seq_t *S_prev = NULL;
static seq_t *S_curr = NULL;

/* Expand the internal tables to at least "limit" and compute a(n) for all new indices */
static void expand(idx_t limit)
{
    if (limit <= max_n)
        return;

    /* Allocate new tables with enough space */
    size_t new_size = (size_t)(limit + 1);

    seq_t *new_a_val = (seq_t *)malloc(new_size * sizeof(seq_t));
    seq_t *new_lucas = (seq_t *)malloc(new_size * sizeof(seq_t));
    seq_t *new_fact  = (seq_t *)malloc(new_size * sizeof(seq_t));
    seq_t *new_b     = (seq_t *)malloc(new_size * sizeof(seq_t));
    seq_t *new_S_prev = (seq_t *)malloc(new_size * sizeof(seq_t));
    seq_t *new_S_curr = (seq_t *)malloc(new_size * sizeof(seq_t));

    /* Copy old data (if any) and zero-fill the rest */
    for (idx_t i = 0; i <= max_n; ++i) {
        new_a_val[i] = a_val ? a_val[i] : (seq_t)0;
        new_lucas[i] = lucas ? lucas[i] : (seq_t)0;
        new_fact[i]  = fact  ? fact[i]  : (seq_t)0;
        new_b[i]     = b     ? b[i]     : (seq_t)0;
        new_S_prev[i] = S_prev ? S_prev[i] : (seq_t)0;
        new_S_curr[i] = (seq_t)0;
    }
    for (idx_t i = max_n + 1; i <= limit; ++i) {
        new_a_val[i] = (seq_t)0;
        new_lucas[i] = (seq_t)0;
        new_fact[i]  = (seq_t)0;
        new_b[i]     = (seq_t)0;
        new_S_prev[i] = (seq_t)0;
        new_S_curr[i] = (seq_t)0;
    }

    /* Free old tables */
    free(a_val);
    free(lucas);
    free(fact);
    free(b);
    free(S_prev);
    free(S_curr);

    a_val = new_a_val;
    lucas = new_lucas;
    fact  = new_fact;
    b     = new_b;
    S_prev = new_S_prev;
    S_curr = new_S_curr;

    /* Initialise base case n = 0 */
    if (max_n == 0) {
        a_val[0] = (seq_t)0;
        lucas[0] = (seq_t)2;   /* Lucas(0) = 2, unused for n>=1 */
        lucas[1] = (seq_t)1;   /* Lucas(1) = 1, will be set below if needed */
        fact[0]  = (seq_t)1;
        b[0]     = (seq_t)0;   /* k=0 not used */
        S_prev[0] = (seq_t)1;  /* S(0,0) = 1 for recurrence */
    }

    /* Compute new entries starting from current max_n + 1 up to limit */
    for (idx_t cur = max_n + 1; cur <= limit; ++cur) {
        /* Compute Lucas, factorial and b = k! * Lucas(k) */
        if (cur == 1) {
            lucas[1] = (seq_t)1;
        } else {
            lucas[cur] = lucas[cur - 1] + lucas[cur - 2];
        }
        fact[cur] = fact[cur - 1] * (seq_t)cur;
        b[cur]    = fact[cur] * lucas[cur];

        /* Stirling numbers: S_curr[k] = S_prev[k-1] + k * S_prev[k] */
        S_curr[0] = (seq_t)0;  /* S(n,0) = 0 for n>=1 */
        for (idx_t k = 1; k <= cur; ++k) {
            S_curr[k] = S_prev[k - 1] + (seq_t)k * S_prev[k];
        }

        /* Compute a(cur) = Σ_k (-1)^(cur-k) * S_curr[k] * b[k] */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t prod = S_curr[k] * b[k];
            if (((cur - k) & 1U) == 0) {
                sum += prod;
            } else {
                sum -= prod;
            }
        }
        a_val[cur] = sum;

        /* Swap S_prev and S_curr for next iteration */
        seq_t *tmp = S_prev;
        S_prev = S_curr;
        S_curr = tmp;
    }

    max_n = limit;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    expand(n);
    return a_val[n];
}

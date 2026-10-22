/*
 * A393756  E.g.f.: exp( x * Sum_{n>=0} n! * 6^n * x^n ).
 * https://oeis.org/A393756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A393756
   a(0) = 1
   a(n) = (n-1)! * Σ_{k=1..n} [ k! * 6^(k-1) * a(n-k) / (n-k)! ],  n ≥ 1
   (repeated use of factorials and powers of 6 with integer arithmetic) */

#include <stdlib.h>

/* ----- static storage for memoisation ----- */
static seq_t *a_vals   = NULL;   /* array of a(n) values   */
static idx_t  a_cap    = 0;      /* capacity of a_vals    */
static idx_t  a_last   = 0;      /* highest index computed */

static seq_t *fact     = NULL;   /* factorial values      */
static idx_t  fact_cap = 0;      /* capacity of fact      */
static idx_t  fact_last= 0;      /* highest factorial index */

static seq_t *pow6     = NULL;   /* powers of 6          */
static idx_t  pow6_cap = 0;      /* capacity of pow6      */
static idx_t  pow6_last= 0;      /* highest pow6 index    */

/* ----- helper: grow an array to hold at least 'needed' elements ----- */
static void *grow_array(void *ptr, idx_t *cap, idx_t needed)
{
    if ((idx_t)*cap > needed) return ptr;
    idx_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    while (new_cap <= needed) new_cap <<= 1;
    *cap = new_cap;
    return realloc(ptr, new_cap * sizeof(seq_t));
}

/* ----- initialise factorials and 6^k up to n ----- */
static void init_fact_pow(idx_t n)
{
    /* factorials */
    fact = (seq_t *)grow_array(fact, &fact_cap, n);
    if (fact_last == 0) {          /* very first initialisation */
        fact[0] = 1;
        fact_last = 0;
    }
    for (idx_t i = fact_last + 1; i <= n; ++i)
        fact[i] = fact[i-1] * (seq_t)i;
    fact_last = n;

    /* powers of 6 */
    pow6 = (seq_t *)grow_array(pow6, &pow6_cap, n);
    if (pow6_last == 0) {          /* very first initialisation */
        pow6[0] = 1;
        pow6_last = 0;
    }
    for (idx_t i = pow6_last + 1; i <= n; ++i)
        pow6[i] = pow6[i-1] * (seq_t)6;
    pow6_last = n;
}

/* ----- initialise a_vals up to n (recurrence) ----- */
static void init_a(idx_t n)
{
    a_vals = (seq_t *)grow_array(a_vals, &a_cap, n);
    if (a_last == 0) {            /* very first initialisation */
        a_vals[0] = 1;
        a_last = 0;
    }

    for (idx_t m = a_last + 1; m <= n; ++m)
    {
        seq_t sum = 0;
        seq_t R   = 1;            /* R = (m-1)!/(m-k)! for current k */

        for (idx_t k = 1; k <= m; ++k)
        {
            seq_t coeff = R * fact[k] * pow6[k-1];
            sum += a_vals[m-k] * coeff;
            if (k < m)            /* update R for next k */
                R *= (seq_t)(m - k);
        }
        a_vals[m] = sum;
    }
    a_last = n;
}

/* ----- public function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    init_fact_pow(n);     /* ensures fact and pow6 arrays are ready */
    init_a(n);            /* ensures a_vals[n] is computed */

    return a_vals[n];
}

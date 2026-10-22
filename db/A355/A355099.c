/*
 * A355099  E.g.f. A(x) satisfies A(x) = 1 - 3 * log(1-x) * A(-log(1-x)).
 * https://oeis.org/A355099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * Sum_{k=1..n} k * |Stirling1(n,k)| * a(k-1) a(0) = 1
 *            (Stirling1: unsigned Stirling numbers of the first kind) The
 *            function expands lazily and memoises results, handling any order
 *            of calls
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---------- static state ------------------------------------ */
    static seq_t *a_cache = NULL;
    static size_t len_a = 0;   /* number of computed a values */
    static size_t cap_a = 0;

    static seq_t *s_prev = NULL; /* Stirling row for previous n */
    static seq_t *s_curr = NULL; /* buffer for next Stirling row */
    static size_t cap_s = 0;
    static idx_t cur_s_n = 0;    /* n for which s_prev is valid */

    /* ---------- initialise on first call ------------------------ */
    if (a_cache == NULL) {
        /* a(0) = 1 */
        cap_a = 1;
        a_cache = (seq_t*)malloc(cap_a * sizeof(seq_t));
        a_cache[0] = (seq_t)1;
        len_a = 1;

        /* Stirling row for n = 0 : s_prev[0] = 1 */
        cap_s = 1;
        s_prev = (seq_t*)malloc(cap_s * sizeof(seq_t));
        s_prev[0] = (seq_t)1;
        cur_s_n  = 0;

        /* s_curr will be allocated lazily when needed */
        s_curr = NULL;
    }

    if (n < len_a) /* already computed */
        return a_cache[n];

    /* ---------- extend cache up to n ----------------------------- */
    for (idx_t new_n = len_a; new_n <= n; ++new_n) {

        /* Ensure Stirling arrays big enough for new_n+1 entries */
        if (cap_s < new_n + 1) {
            size_t new_cap = new_n + 1;
            seq_t *new_prev = (seq_t*)realloc(s_prev, new_cap * sizeof(seq_t));
            seq_t *new_curr = (seq_t*)realloc(s_curr, new_cap * sizeof(seq_t));
            s_prev = new_prev;
            s_curr = new_curr;
            cap_s  = new_cap;
        }

        /* Compute Stirling row for new_n (s_curr) using s_prev */
        s_curr[0] = (seq_t)0;
        for (idx_t k = 1; k <= new_n; ++k) {
            seq_t term1 = (seq_t)0;
            if (k <= cur_s_n)                /* (n-1) * s_prev[k] if defined */
                term1 = (seq_t)(new_n - 1) * s_prev[k];
            seq_t term2 = s_prev[k - 1];      /* s_prev[k-1] is always defined   */
            s_curr[k] = term1 + term2;
        }

        /* Compute a(new_n) = 3 * sum_{k=1..new_n} k * s_curr[k] * a(k-1) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= new_n; ++k) {
            seq_t term   = (seq_t)k * s_curr[k];
            term         = term * a_cache[k - 1];
            sum          = sum + term;
        }
        seq_t a_value = sum * ((seq_t)3);

        /* Extend a_cache if needed */
        if (cap_a <= len_a) {
            size_t new_cap = (cap_a == 0) ? 1 : cap_a * 2;
            seq_t *new_a = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (a_cache) memcpy(new_a, a_cache, len_a * sizeof(seq_t));
            free(a_cache);
            a_cache = new_a;
            cap_a   = new_cap;
        }

        a_cache[len_a++] = a_value;

        /* Swap s_prev and s_curr for next iteration */
        seq_t *tmp = s_prev;
        s_prev = s_curr;
        s_curr = tmp;
        cur_s_n = new_n;
    }

    return a_cache[n];
}

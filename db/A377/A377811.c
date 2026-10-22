/*
 * A377811  E.g.f. satisfies A(x) = exp(x * A(x)) / (1 - x)^3.
 * https://oeis.org/A377811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..n} (k+1)^{k-1} * binomial(n+2k+2, n-k)/k!
 *            Equivalent recurrence from the e.g.f.: B_0 = 1, a_0 = 1 B_n =
 *            Σ_{k=1..n} binomial(n-1, k-1) * k * a_{k-1} * B_{n-k} a_n = B_n
 *            + 3*C(n,1)*a_{n-1} - 6*C(n,2)*a_{n-2} + 6*C(n,3)*a_{n-3}
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*====================================================================*/
#include <stdlib.h>
#include <stddef.h>

static seq_t *a_cache  = NULL;
static size_t a_cap    = 0;
static idx_t  a_size   = 0;

static seq_t *b_cache  = NULL;
static size_t b_cap    = 0;
static idx_t  b_size   = 0;

/*--------------------------------------------------------------------*/
static void grow_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}
/*--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Grow storage for this index if necessary */
    grow_array(&a_cache, &a_cap, (size_t)(n + 1));
    grow_array(&b_cache, &b_cap, (size_t)(n + 1));

    /* Return cached value if already known */
    if (a_size > n) return a_cache[n];

    /* Initialise base case if this is the first call */
    if (a_size == 0) {
        a_cache[0] = (seq_t)1;          /* a(0) = 1 */
        b_cache[0] = (seq_t)1;          /* B_0 = 1   */
        a_size = b_size = 1;
    }

    /* Compute new values up to n */
    for (idx_t curr = a_size; curr <= n; ++curr) {
        /* ----- Compute B_curr ----------------------------------- */
        seq_t sum = 0;
        seq_t bin = 1;                /* binomial(curr-1, 0) */
        for (idx_t k = 1; k <= curr; ++k) {
            seq_t f_k = (seq_t)k * a_cache[k - 1];      /* k * a_{k-1} */
            seq_t term = f_k * b_cache[curr - k];
            term = term * bin;                          /* multiply by C(curr-1, k-1) */
            sum += term;
            /* Update binomial for next k: C(curr-1, k) */
            if (k < curr) {
                bin = (bin * (seq_t)(curr - k)) / (seq_t)k;
            }
        }
        b_cache[curr] = sum;

        /* ----- Compute a_curr ----------------------------------- */
        seq_t temp = sum;  /* start with B_curr */

        if (curr >= 1) {
            seq_t c1 = (seq_t)curr;
            temp += (seq_t)3 * c1 * a_cache[curr - 1];
        }
        if (curr >= 2) {
            seq_t n128 = (seq_t)curr;
            seq_t c2 = (n128 * (n128 - 1)) / 2;          /* C(curr,2) */
            temp -= (seq_t)6 * c2 * a_cache[curr - 2];
        }
        if (curr >= 3) {
            seq_t n128 = (seq_t)curr;
            seq_t c3 = (n128 * (n128 - 1) * (n128 - 2)) / 6; /* C(curr,3) */
            temp += (seq_t)6 * c3 * a_cache[curr - 3];
        }

        a_cache[curr] = temp;
    }

    a_size = b_size = n + 1;
    return a_cache[n];
}

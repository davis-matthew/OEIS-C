/*
 * A399065  Expansion of e.g.f. 1/(1 - x*cosh(x)^3).
 * https://oeis.org/A399065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = n * ∑_{j=0}^{n-1} B[n-1-j] * binomial(n-1, j) *
 *            a(j) where B[m] = 0 for odd m, and for even m = 2e: B[2e] =
 *            (3^(2e) + 3) / 4 ( integer ) This yields the sequence A399065
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The types seq_t and idx_t are defined by the build system. */
/* No other includes or definitions are allowed. */

/* Helper function: ensures we have capacity for at least 'need' elements
   in an array of seq_t.  The array is reallocated in place.  The
   'cap' points to the current capacity. */
static void ensure_capacity_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = need * 2;
        if (new_cap < need) new_cap = need;  /* protect overflow */
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* The global state used for memoisation and for pre‑computing the B array */
static seq_t *a_vals = NULL;      /* sequence a(n) */
static size_t a_cap  = 0;          /* capacity of a_vals */
static size_t a_len  = 0;          /* next free index in a_vals */

static seq_t *B_vals = NULL;      /* coefficients B[m] for m up to current size-1 */
static size_t B_cap  = 0;          /* capacity of B_vals */
static size_t B_len  = 0;          /* next free index in B_vals */

static idx_t pow3_index = 0;       /* current exponent of 3 (for B computation) */
static seq_t pow3_value;           /* 3^pow3_index */

/* Initialise the global arrays on the first call */
static void init_once(void)
{
    static bool init_done = false;
    if (init_done) return;
    init_done = true;

    /* allocate space for a(0) */
    a_cap = 1;
    a_vals = (seq_t *)malloc(a_cap * sizeof(seq_t));
    a_vals[0] = (seq_t)1;  /* a(0) = 1 */
    a_len = 1;

    /* allocate space for B[0] */
    B_cap = 1;
    B_vals = (seq_t *)malloc(B_cap * sizeof(seq_t));
    B_vals[0] = (seq_t)1;  /* B[0] = (3^0 + 3)/4 = 1 */
    B_len = 1;

    pow3_index = 0;
    pow3_value = (seq_t)1;   /* 3^0 */
}

/* Compute B values up to index 'm' (inclusive).  The B array must have
   enough capacity.  The pow3_value and pow3_index are updated. */
static void compute_B_up_to(idx_t m)
{
    /* Ensure capacity for B_vals */
    ensure_capacity_seq(&B_vals, &B_cap, m + 1);

    for (idx_t k = B_len; k <= m; ++k) {
        /* Update pow3_value to 3^k */
        pow3_value = pow3_value * (seq_t)3;
        pow3_index = k;

        if ((k & 1ULL) == 0) {               /* k even */
            seq_t temp = pow3_value + (seq_t)3;   /* 3^k + 3 */
            B_vals[k] = temp / (seq_t)4;          /* divide by 4 */
        } else {
            B_vals[k] = (seq_t)0;   /* odd indices are zero */
        }
    }
    B_len = m + 1;
}

/* Main function requested by the problem statement */
seq_t a(idx_t n)
{
    init_once();

    /* Ensure B up to n-1 (since we need B[n-1-j] terms) */
    if (n > 0) {
        if (n - 1 >= B_len) {
            compute_B_up_to(n - 1);
        }
    }

    /* Extend the a sequence up to n */
    if (n >= a_len) {
        /* Allocate or grow the array */
        ensure_capacity_seq(&a_vals, &a_cap, n + 1);

        /* Compute new terms from a_len up to n */
        for (idx_t curr = a_len; curr <= n; ++curr) {
            if (curr == 0) {                 /* already initialised */
                a_vals[0] = (seq_t)1;
                continue;
            }

            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;           /* binomial(curr-1, 0) */

            for (idx_t j = 0; j < curr; ++j) {
                idx_t m = curr - 1 - j;

                if ((m & 1ULL) == 0 && B_vals[m] != (seq_t)0) {
                    /* term = B[m] * binom * a[j] */
                    seq_t term = B_vals[m];
                    term = term * binom;
                    term = term * a_vals[j];
                    sum = sum + term;
                }

                /* Update binomial(curr-1, j+1) */
                if (j + 1 < curr) {
                    seq_t num = (seq_t)(curr - 1 - j);
                    seq_t den = (seq_t)(j + 1);
                    binom = binom * num / den;
                }
            }

            a_vals[curr] = (seq_t)curr * sum;
        }
        a_len = n + 1;
    }

    return a_vals[n];
}

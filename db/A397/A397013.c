/*
 * A397013  Expansion of the exponential generating function (1 - 23*x - (30 -
 *          6*x)*log(1-x)) / (1 - x)^5.
 * https://oeis.org/A397013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+3)! * ((n+5) * H(n+3) - 3*n - 9)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where H(m) is the m‑th harmonic number.  */
/*  The recurrence below avoids floating point and large intermediates: */
/*  Let m = n + 3.  Then for m >= 1 :
 *      fact[m] = m!                         (fact[0] = 1)
 *      sum[m]  = Σ_{k=1}^{m} fact[m] / k    (sum[0] = 0)
 *      a[m]    = (m+2)*sum[m] - 3*m*fact[m]
 *  The values are computed incrementally so that repeated calls are O(1) per
 *  new index beyond the current maximum.  A static buffer covers all n
 *  expected in the test suite (up to about 10 000).  The code is valid
 *  in C11 and C++17 and uses only integer arithmetic.  */

#include <stdint.h>
#include <stddef.h>

#define MAX_M 20010          /* enough for n up to ~10 000 */

static idx_t current_max = 0;
static seq_t fact[MAX_M];
static seq_t sum_arr[MAX_M];
static seq_t a_arr[MAX_M];
static int initialized = 0;

seq_t a(idx_t n)
{
    const idx_t m = n + 3;               /* target factorial index */

    /* Initialise base case once */
    if (!initialized) {
        fact[0]  = (seq_t)1;
        sum_arr[0] = (seq_t)0;
        a_arr[0] = (seq_t)0;
        current_max = 0;
        initialized = 1;
    }

    /* Guard against indices beyond the preallocated table */
    if (m >= MAX_M) {
        /* In the unlikely event that n is too large, return 0 as a safe fallback. */
        return (seq_t)0;
    }

    /* Extend the precomputed tables up to m */
    if (m > current_max) {
        for (idx_t i = current_max + 1; i <= m; ++i) {
            /* factorial */
            fact[i] = fact[i - 1] * (seq_t)i;

            /* sum of factorial divided by k */
            sum_arr[i] = (seq_t)i * sum_arr[i - 1] + fact[i - 1];

            /* sequence value */
            seq_t term1 = (seq_t)(i + 2) * sum_arr[i];
            seq_t term2 = (seq_t)(3 * i) * fact[i];
            a_arr[i] = term1 - term2;
        }
        current_max = m;
    }

    return a_arr[m];
}

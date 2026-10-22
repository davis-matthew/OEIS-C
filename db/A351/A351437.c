/*
 * A351437  G.f. A(x) satisfies: A(x) = 1 + x + x^2 * A(x/(1 - x)) / (1 -
 *          x)^2.
 * https://oeis.org/A351437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 1; a(n) = Sum_{k=0..n-2} binomial(n-1, k+1) * a(k)
 *            for n >= 2
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoizes results in static arrays.
   Binomial coefficients are updated row‑by‑row using Pascal's rule
   in place.  It runs in O(n²) time and uses O(n) memory. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache for a(0..max) and binomial row C[0..max] */
    static seq_t *a_cache   = NULL; /* a[0..max_n]            */
    static size_t a_cap     = 0;    /* current capacity of a_cache   */
    static idx_t  a_max     = 0;    /* maximum index already computed */

    static seq_t *C         = NULL; /* current row C( i , j )  */
    static size_t C_cap     = 0;    /* capacity of C array         */
    static idx_t  C_row     = 0;    /* current row index (equals
                                      a_max, because after computing
                                      a[a_max] we have C(row=a_max) )*/

    static bool initialized = false;

    /* Initialisation: compute a[0] and a[1], and prepare C for row 1 */
    if (!initialized) {
        /* Allocate space for a[0] and a[1] */
        a_cap   = 2;
        a_cache = (seq_t *) malloc(a_cap * sizeof(seq_t));
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)1;
        a_max     = 1;   /* we have a[0] and a[1] */

        /* Allocate and initialise C for row 1: C(1,0)=1, C(1,1)=1 */
        C_cap   = 2;
        C       = (seq_t *) malloc(C_cap * sizeof(seq_t));
        C[0]    = (seq_t)1;
        C[1]    = (seq_t)1;
        C_row   = 1;     /* C represents row 1 */

        initialized = true;
    }

    /* Return cached value if available */
    if (n <= a_max)
        return a_cache[n];

    /* Compute values up to n */
    for (idx_t m = a_max + 1; m <= n; ++m) {
        /* Compute a[m] = sum_{j=1}^{m-1} C(j) * a[j-1] */
        seq_t sum = 0;
        for (idx_t j = 1; j <= m - 1; ++j) {
            sum += C[j] * a_cache[j - 1];
        }

        /* Ensure enough capacity for a_cache */
        if (m >= a_cap) {
            size_t new_cap = a_cap ? a_cap * 2 : 4;
            if (new_cap <= m) new_cap = m + 1;
            a_cache = (seq_t *) realloc(a_cache, new_cap * sizeof(seq_t));
            a_cap   = new_cap;
        }
        a_cache[m] = sum;

        /* Prepare next row of binomial coefficients: C(m, j) */
        idx_t newSize = m + 1;   /* row index is m, size is m+1 */
        if (newSize > C_cap) {
            size_t new_cap = C_cap ? C_cap * 2 : 4;
            if (new_cap < newSize) new_cap = newSize;
            C      = (seq_t *) realloc(C, new_cap * sizeof(seq_t));
            C_cap  = new_cap;
        }
        /* New last element starts as 0 before Pascal update */
        C[newSize - 1] = (seq_t)0;
        for (idx_t j = newSize - 1; j >= 1; --j)
            C[j] += C[j - 1];

        /* Update tracking variables */
        C_row   = m;   /* C now holds row m   */
        a_max   = m;   /* a_cache[m] computed */
    }

    return a_cache[n];
}

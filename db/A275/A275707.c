/*
 * A275707  Number of partial functions f:{1,2,...,n}->{1,2,...,n} such that
 *          every element in the domain of definition of f is mapped to a
 *          fixed point or to an element that is undefined by f.
 * https://oeis.org/A275707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A275707:  a(n) = Σ_{k=1..n} 2*k*binomial(n-1,k-1)*a(n-k)
   a(0) = 1
   Uses a memoised dynamic programme that runs in O(n²) time
   (≈ ½·n² big‑integer operations).                                 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* calloc, free, exit */
#ifdef __cplusplus
#include <new>        /* placement new for C++ builds */
#endif

/* External integer types are supplied by the build system; do not
   define them yourself.                                       */
static seq_t *A = NULL;          /* memoised table of a[0..]      */
static size_t A_cap = 0;         /* number of allocated elements   */
static size_t A_computed = 0;    /* first index not yet computed   */

/* Allocate or grow the cache to hold indices 0..max_n.          */
static void ensure_capacity(idx_t max_n)
{
    size_t need = (size_t)(max_n + 1);
    if (need <= A_cap) return;      /* already enough space          */

    /* Allocate a new array of seq_t objects.                     */
    seq_t *newA = NULL;
#ifdef __cplusplus
    newA = new seq_t[need];          /* default constructed in C++  */
#else
    newA = calloc(need, sizeof(seq_t));          /* zero‑init in C            */
    if (!newA) exit(EXIT_FAILURE);
#endif

    /* Copy data from the old array if it exists.                 */
    if (A_cap > 0) {
        for (size_t i = 0; i < A_cap; ++i)
            newA[i] = A[i];
#ifdef __cplusplus
        delete[] A;
#else
        free(A);
#endif
    }

    /* Hook up the new array.                                       */
    A = newA;
    A_cap = need;

    /* Initialise the base case if this is the first allocation.   */
    if (A_cap == 1)
        A[0] = (seq_t)1;
}

/* Compute a(n).  Memoised, but works correctly even if called
   with arguments out of order.                               */
seq_t a(idx_t n)
{
    /* Make sure the array is large enough.                      */
    ensure_capacity(n);

    /* Determine the first index that has not yet been computed. */
    idx_t start = (idx_t)A_computed;
    if (start < 1) start = 1;          /* skip base case */
    if (start <= n) {
        for (idx_t i = start; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;   /* C(i-1,0) */

            for (idx_t k = 1; k <= i; ++k) {
                /* term = C(i-1,k-1) * (2*k) * a(i-k)          */
                seq_t term = binom;
                term *= (seq_t)(2 * k);
                term *= A[i - k];
                sum += term;

                /* Update binom to C(i-1,k) for the next step */
                if (k < i) {
                    binom *= (seq_t)(i - k);
                    binom /= (seq_t)k;
                }
            }
            A[i] = sum;
        }
        A_computed = (size_t)(n + 1);
    }
    return A[n];
}

/*
 * A277405  a(n) equals the constant term in the sum of all permutations of
 *          compositions of functions (1 + k*x) for k=1..n.
 * https://oeis.org/A277405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! + Σ_{m=1..n} m! · (n-m)! · e_m(1,…,n) where e_m are
 *            the elementary symmetric sums of the integers 1,2,…,n. This
 *            follows from expanding the compositions of the linear maps
 *            f_k(x)=1+kx
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  External types supplied by the build system.
    seq_t   – unsigned integer type of sufficient size
    idx_t   – unsigned integer (usually uint64_t) used for indices   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate arrays for factorials (size n+1) and elementary
       symmetric sums e[0..n].  Using calloc ensures zero-initialised
       storage even when seq_t is an arbitrary‑precision class.   */
    seq_t *fact = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *e    = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!fact || !e)
        return (seq_t)0;  /* allocation failure – return zero            */

    /* factorial[0] = 1, factorial[i] = i!  */
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* elementary symmetric sums:
       e[0] = 1;  for k = 1..n:
           for i = k downto 1:  e[i] += k * e[i-1]   */
    e[0] = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
        for (idx_t i = k; i >= 1; --i)
            e[i] = e[i] + (seq_t)k * e[i - 1];

    /* sum_{k=0}^{n-1} k! · (n-k)! · e_k   */
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k < n; ++k)
        sum += fact[k] * fact[n - k] * e[k];

    free(fact);
    free(e);
    return sum;
}

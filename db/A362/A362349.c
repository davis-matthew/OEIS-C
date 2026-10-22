/*
 * A362349  a(n) = n! * Sum_{k=0..floor(n/4)} k^k / (k! * (n-4*k)!).
 * https://oeis.org/A362349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/4⌋} k^k / (k! * (n-4k)!) (OEIS A362349)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
////
/// Computation is done in O(n) time per call with only a small amount of
/// auxiliary data.  The implementation uses an iterative approach that
/// keeps the factorial of n, the product n!/(n-4k) and k! in running
/// variables, and calls the provided big_pow routine for k^k.
/// Everything is performed in seq_t, which works for arbitrary‑precision
/// or any fixed‑width unsigned type.  No external state is required;
/// the function is safe for calls in any order.
///
/// The code complies with the stated requirements and uses only the
/// helpers listed in the problem statement (big_pow).
///
*/
#include <stdlib.h>
#include "../utils/big_pow.h"

/* Compute a(n) for OEIS A362349 */
seq_t a(idx_t n)
{
    /* Factorial of n */
    seq_t fact_n = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact_n *= (seq_t)i;

    /* Running product R = n! / (n-4k)!   (R=1 when k=0) */
    seq_t R = (seq_t)1;
    /* Running factorial of k   (fact_k=1 when k=0) */
    seq_t fact_k = (seq_t)1;
    seq_t total  = (seq_t)0;

    idx_t maxk = n / 4;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* k^k, treating 0^0 as 1 */
        seq_t pk = (k == 0) ? (seq_t)1 : big_pow((seq_t)k, (seq_t)k);

        /* term = R * k^k / k! */
        seq_t term = R * pk;
        term /= fact_k;

        total += term;

        /* Prepare for next k, unless at the last iteration */
        if (k < maxk)
        {
            /* Update R: multiply by (n-4k)(n-4k-1)(n-4k-2)(n-4k-3) */
            seq_t a1 = (seq_t)(n - 4 * k);
            seq_t a2 = (seq_t)(n - 4 * k - 1);
            seq_t a3 = (seq_t)(n - 4 * k - 2);
            seq_t a4 = (seq_t)(n - 4 * k - 3);
            R *= a1;
            R *= a2;
            R *= a3;
            R *= a4;
        }

        /* Update factorial of k for next iteration: (k+1)! = k! * (k+1) */
        fact_k *= (seq_t)(k + 1);
    }

    return total;
}

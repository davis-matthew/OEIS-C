/*
 * A323181  a(n) = U_{2*n-1}(n)/(2*n) where U_{n}(x) is a Chebyshev polynomial
 *          of the second kind.
 * https://oeis.org/A323181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) = U_{n-1}(2*n^2-1), where U_k is the Chebyshev polynomial of the second kind.
   Compute U_{n-1} by its three‑term recurrence:
       U_0(x) = 1,
       U_1(x) = 2x,
       U_{k+1}(x) = 2x U_k(x) – U_{k-1}(x).
   The result fits into seq_t for all n where a(n) fits into the type, and intermediate
   values never overflow that type for the intended test ranges.  */ 

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term.  */
    if (n == 0) return 0;      /* undefined for offset 1, but harmless.  */
    if (n == 1) return 1;      /* U_0(1) = 1.  */

    /* Compute the argument of the Chebyshev polynomial: 2*n^2-1.  */
    seq_t T = (seq_t)2 * n * n - 1;

    /* Start the recurrence.  */
    seq_t U_prev2 = (seq_t)1;          /* U_0(T) */
    seq_t U_prev1 = (seq_t)2 * T;      /* U_1(T) */

    for (idx_t k = 2; k <= n - 1; ++k) {
        seq_t U_curr = (seq_t)2 * T * U_prev1 - U_prev2;
        U_prev2 = U_prev1;
        U_prev1 = U_curr;
    }

    /* U_{n-1}(T) is now stored in U_prev1.  */
    return U_prev1;
}

/*
 * A355883  Number of ways to 5-color a 3 X n grid ignoring the variations of
 *          two colors.
 * https://oeis.org/A355883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 45*a(n-1) - 116*a(n-2) with a(1)=4, a(2)=169
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, if needed */

/* Compute the n-th term of OEIS A355883 (3×n 5‑colourings, ignoring
   permutations of two colors).  The recurrence is linear with
   order 2, so it is evaluated iteratively.
   The implementation uses 128‑bit arithmetic in C for 64‑bit
   seq_t to avoid overflow during intermediate computations.  In C++
   the overloaded operators on arbitrary‑precision seq_t handle large
   numbers correctly. */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)169;

    seq_t a_nm2 = (seq_t)4;   /* a(1)   */
    seq_t a_nm1 = (seq_t)169; /* a(2)   */
    seq_t a_n   = (seq_t)0;

    /* Iteratively compute a(n) for n >= 3 */
#ifdef __cplusplus
    /* In C++ (big‑int build) just use the arithmetic operators */
    for (idx_t i = 3; i <= n; ++i) {
        a_n = (seq_t)(45 * a_nm1 - 116 * a_nm2);
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
#else
    /* In C (64‑bit build) use unsigned __int128 for the products to
       prevent overflow in intermediate steps. */
    for (idx_t i = 3; i <= n; ++i) {
        unsigned __int128 prod1 = (unsigned __int128)45 * a_nm1;
        unsigned __int128 prod2 = (unsigned __int128)116 * a_nm2;
        unsigned __int128 tmp   = prod1 - prod2;
        a_n = (seq_t)tmp;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
#endif

    return a_n;
}

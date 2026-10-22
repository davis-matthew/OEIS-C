/*
 * A398535  a(n) is the trace of the product of two Hankel n X n matrices M_n
 *          and N_n whose M[i,j] generic elements are respectively (i+j)^2 and
 *          (i+j)^3.
 * https://oeis.org/A398535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n + 1)^2*(18*n^3 + 26*n^2 + 5*n - 1)/6
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 19, u64 n <= 482,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Promote index to 128–bit to avoid overflow in intermediate
       computations. GCC/Clang expose the GNU extension unsigned __int128. */
    unsigned __int128 N   = (unsigned __int128)n;
    unsigned __int128 N2  = N * N;                 /* n^2            */
    unsigned __int128 Np1 = N + 1;
    unsigned __int128 Np2 = Np1 * Np1;            /* (n+1)^2        */
    unsigned __int128 N3  = N2 * N;                /* n^3            */
    unsigned __int128 poly = 18u * N3             /* 18*n^3         */
                           + 26u * N2           /* 26*n^2         */
                           + 5u  * N            /* 5*n            */
                           - 1u;                /* -1             */
    unsigned __int128 prod = N2 * Np2;            /* n^2*(n+1)^2    */
    prod = prod * poly;                          /* * polynomial   */
    prod = prod / 6u;                            /* divide by 6    */
    return (seq_t)prod;
}

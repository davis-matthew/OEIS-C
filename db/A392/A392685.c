/*
 * A392685  a(n) = numerator( (1/(4*n + 2))*Sum_{i=0..2*n} (-1)^i/(2*i+1) ).
 * https://oeis.org/A392685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator( (1/(4*n + 2))*Sum_{i=0..2*n} (-1)^i/(2*i+1) )
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the recurrence:  S_n = S_{n-1} - 2/((4n-1)(4n+1))  with  S_0 = 1  */
/*   which leads to the integer recurrence for the reduced numerator N_n:  */
/*   N_0 = 1, D_0 = 1                                            */
/*   for k ≥ 1 :  f = (4k‑1)(4k+1)                               */
/*                N_k = N_{k-1}·f – 2·D_{k-1}                    */
/*                D_k = D_{k-1}·f                               */
/*                reduce (N_k,D_k) by gcd                          */
/*   The desired term is then N_n / gcd(N_n, 4n+2).            */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t N = 1;
    seq_t D = 1;

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t f1 = (seq_t)(4ULL * k - 1);
        seq_t f2 = (seq_t)(4ULL * k + 1);
        seq_t f  = f1 * f2;                     /* (4k-1)*(4k+1)  */

        seq_t newN = N * f - 2 * D;             /* N_{k}         */
        seq_t newD = D * f;                     /* D_{k}         */

        seq_t g = gcd(newN, newD);              /* reduce fraction */
        N = newN / g;
        D = newD / g;
    }

    seq_t denom_mult = (seq_t)(4ULL * n + 2);
    seq_t g2 = gcd(N, denom_mult);
    return N / g2;
}

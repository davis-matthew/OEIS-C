/*
 * A380083  a(n) = Pell(n^2)/Pell(n).
 * https://oeis.org/A380083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Pell(n^2) / Pell(n) (offset 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 48 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute the pair (P_n , P_{n+1}) of Pell numbers.
   Uses fast doubling and the identities:
      P_{2k}   = 2 * P_k * (P_{k+1} - P_k)
      P_{2k+1} = P_{k+1}^2 + P_k^2
   which follow from the matrix representation
      M = [[2,1],[1,0]]  and  M^n = [[P_{n+1},P_n],[P_n,P_{n-1}]].
*/
static void pell_pair(idx_t n, seq_t *pn, seq_t *pn1)
{
    if (n == 0)
    {
        *pn  = 0;   /* Pell(0) */
        *pn1 = 1;   /* Pell(1) */
        return;
    }
    idx_t k = n >> 1;
    seq_t pk, qk;            /* pk  = P_k     , qk = P_{k+1}   */
    pell_pair(k, &pk, &qk);

    /* Two * (qk - pk)  = 2 * (P_{k+1} - P_k) */
    seq_t two_q_minus_p = (qk - pk) << 1;

    /* P_{2k}   = 2 * P_k * (P_{k+1} - P_k)          */
    seq_t p_even = pk * two_q_minus_p;
    /* P_{2k+1} = P_{k+1}^2 + P_k^2                 */
    seq_t p_odd = qk * qk + pk * pk;

    if ((n & 1ULL) == 0)          /* n is even : return (P_{2k},P_{2k+1}) */
    {
        *pn  = p_even;
        *pn1 = p_odd;
    }
    else                          /* n is odd  : return (P_{2k+1},P_{2k+2}) */
    {
        /* P_{2k+2} = 2*P_{2k+1} + P_{2k}               */
        *pn  = p_odd;
        *pn1 = (p_odd << 1) + p_even;
    }
}

/* Sequence function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    /* For the requested offset (1) no values are defined for n==0. */
    if (n == 0) return 0;

    /* Compute Pell(n) */
    seq_t p_n, p_n1;
    pell_pair(n, &p_n, &p_n1);

    /* Compute Pell(n^2) */
    idx_t n2 = n * n;          /* n <= 10000 so the product fits in idx_t */
    seq_t p_n2, p_n2_1;
    pell_pair(n2, &p_n2, &p_n2_1);

    /* a(n) = Pell(n^2) / Pell(n)  ; division is exact         */
    return p_n2 / p_n;
}

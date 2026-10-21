/*
 * A271833  Expansion of (1 + 2*x + 2*x^2 + 2*x^3 - 5*x^4 + 2*x^5 + 2*x^6 +
 *          2*x^7)/((1 - x)^2*(1 + x + x^2 + x^3 + x^4 + x^5 + x^6 + x^7)).
 * https://oeis.org/A271833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2*n + 6*floor(n/8) - 7*floor(n/4)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A271833  –  OEIS  */
#include <stdint.h>

/* External linkage – no static keyword */
seq_t a(idx_t n)
{
    /* Use 128‑bit integer for intermediate calculations to avoid overflow
       before truncation to the target seq_t type. */
    __uint128_t nn = (uint64_t)n;          /* convert idx_t to 128‑bit */
    __uint128_t result =
        ( (__uint128_t)1 )
        + ( (__uint128_t)2 * nn )
        + ( (__uint128_t)6 * (nn / 8) )
        - ( (__uint128_t)7 * (nn / 4) );

    return (seq_t) result;                  /* cast to the required type */
}

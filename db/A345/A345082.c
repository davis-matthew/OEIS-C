/*
 * A345082  Number of elements of order n in R/Z X Z/2Z.
 * https://oeis.org/A345082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   if n is odd: a(n)=φ(n); if n≡0 (mod 4): a(n)=2φ(n); if n≡2 (mod
 *            4): a(n)=2φ(n)+φ(n/2)
 * Range:     u8 n <= 177, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the phi function from "../utils/phi.h" */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    seq_t phi_n = phi(n);
    if (n & 1u)                               /* n odd      */
        return phi_n;
    else
    {
        if ((n & 3u) == 0u)                   /* n ≡ 0 (mod 4) */
            return phi_n + phi_n;             /* 2φ(n)       */
        else                                  /* n ≡ 2 (mod 4) */
            return phi_n + phi_n + phi(n / 2);
    }
}

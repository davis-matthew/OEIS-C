/*
 * A325892  The successive approximations up to 2^n for the 2-adic integer
 *          3^(1/5).
 * https://oeis.org/A325892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1; for n>1, a(n)=a(n-1) if a(n-1)^5 ≡ 3 (mod 2^n),
 *            else a(n)=a(n-1)+2^(n-1)
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the nth term of OEIS A325892 (successive
   2-adic approximations to 3^(1/5)). */
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 0 and 1 are the initial terms */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Start from a(1) = 1 and lift the solution to higher moduli. */
    seq_t a_val = 1;            /* a(1) */
    for (idx_t k = 2; k <= n; ++k) {
        /* Compute a_val^5 */
        seq_t p = a_val;
        p *= a_val;   /* a^2 */
        p *= a_val;   /* a^3 */
        p *= a_val;   /* a^4 */
        p *= a_val;   /* a^5 */

        /* Check divisibility of (a^5 - 3) by 2^k */
        seq_t diff  = p - 3;
        seq_t mask  = pow2(k) - 1;   /* lower k bits mask */

        if ((diff & mask) != 0) {
            /* If not divisible, lift by adding 2^(k-1) */
            a_val += pow2(k - 1);
        }
        /* else a_val remains the same */
    }
    return a_val;
}

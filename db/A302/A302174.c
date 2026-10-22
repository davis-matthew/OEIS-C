/*
 * A302174  Smallest solution x of x^n + y^(n+1) = z^(n+2), x, y, z >= 1.
 * https://oeis.org/A302174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   for n≥3 odd: a(n) = 2^((n+1)^2/2) for n≥6 even: a(n) =
 *            2^(n*(n+1)/2)·3^(n+2) special low indices: a(0)=1, a(1)=2,
 *            a(2)=27, a(4)=472392
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* handle the few low‑index special cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)27;
    if (n == 4) return (seq_t)472392;

    /* odd n ≥ 3: 2^((n+1)^2/2) */
    if (n & 1ULL) {
        idx_t np1 = n + 1;                     /* n+1  */
        idx_t exp = (np1 * np1) / 2;           /* (n+1)^2/2 */
        return pow2(exp);                      /* 2^exp   */
    }
    /* even n ≥ 6: 2^(n*(n+1)/2)·3^(n+2) */
    else {
        idx_t e2 = (n * (n + 1)) / 2;          /* n*(n+1)/2   */
        idx_t e3 = n + 2;                      /* n+2         */
        return pow2(e2) * pow3(e3);            /* 2^e2 * 3^e3 */
    }
}

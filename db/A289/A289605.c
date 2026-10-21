/*
 * A289605  a(n) is the number of permutations of length n that avoid the
 *          pattern 321 and the mesh pattern (12, 341).
 * https://oeis.org/A289605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n) - 2^(n-1) - n + 7 for n > 4 with a(0)=1,
 *            a(1)=1, a(2)=1, a(3)=3, a(4)=7
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/catalan.h"   /* Provides: seq_t catalan(idx_t k); */
#include "../utils/pow2.h"      /* Provides: seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    /* Base cases for n = 0..4 */
    static const seq_t base[5] = {
        (seq_t)1, /* a(0) */
        (seq_t)1, /* a(1) */
        (seq_t)1, /* a(2) */
        (seq_t)3, /* a(3) */
        (seq_t)7  /* a(4) */
    };

    if (n <= 4)
        return base[n];

    /* For n > 4 use the explicit formula */
    seq_t res = catalan(n);          /* Catalan(n) */
    seq_t two_pow = pow2(n - 1);     /* 2^(n-1) */
    res -= two_pow;                  /* res -= 2^(n-1) */
    res -= (seq_t)n;                 /* res -= n   */
    res += (seq_t)7;                 /* res += 7   */

    return res;
}

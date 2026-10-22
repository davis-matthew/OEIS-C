/*
 * A368777  a(n) is the largest divisor of n that is a term of the sequence
 *          A003418, the least common multiple of the first k natural numbers.
 * https://oeis.org/A368777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest divisor of n that is a term of A003418 (the least
 *            common multiple of 1,2,…,k)
 * Range:     u8 n <= 419, u16 n <= 5040, u32 n <= 5040, u64 n <= 5040,
 *            u128 n <= 5040, big any n
 * Helpers:   _seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/_seq_gcd.h"

/* Euclidean algorithm for seq_t. */

seq_t a(idx_t n)
{
    /* n is ≥ 1 for a(1) onward.  */
    if (n == (idx_t)0) return 1;        /* defensive for n==0 */

    idx_t n_idx = n;                    /* keep original index        */

    /* Storage for lcm(1..k) values that do not exceed n.  */
    seq_t lcms[64];
    size_t cnt = 0;
    lcms[cnt++] = 1;                    /* lcm(1..1) = 1              */
    seq_t cur = 1;                      /* current lcm                */

    for (size_t k = 2; ; ++k) {
        seq_t kcoef = (seq_t)k;
        seq_t g = _seq_gcd(cur, kcoef);

        /* next = cur / g * k */
        seq_t t = cur / g;
        uintmax_t t_big = (uintmax_t)t;
        uintmax_t k_big = (uintmax_t)kcoef;

        /* If the product would exceed n, no larger lcm will divide n. */
        if (t_big > (uintmax_t)n_idx / k_big)
            break;

        uintmax_t next_big = t_big * k_big;
        seq_t next = (seq_t)next_big;

        cur = next;
        if (cur > (seq_t)n_idx)          /* avoid storing lcm > n         */
            break;
        lcms[cnt++] = cur;

        /* Safety: prevent overflow of the static array.          */
        if (cnt >= sizeof(lcms) / sizeof(lcms[0]))
            break;
    }

    /* Return the largest stored lcm that divides n. */
    for (size_t i = cnt; i > 0; --i) {
        seq_t d = lcms[i - 1];
        if (n_idx % (idx_t)d == 0)
            return d;
    }

    return 1;                            /* fallback (should never happen) */
}

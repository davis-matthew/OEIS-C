/*
 * A287729  The c-fusc function c(n) = a(n): a(1)=1, a(2n) = s(n), a(2n+1) =
 *          s(n)+s(n+1), where s(n) = A287730(n).
 * https://oeis.org/A287729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A287729:  c-fusc sequence
 *
 *  The values c(n) are defined by the mutual diatomic recurrence pair
 *  with s(n):
 *
 *      c(1) = 1,    s(1) = 0
 *      c(2n)   = s(n)
 *      c(2n+1) = s(n) + s(n+1)
 *      s(2n)   = c(n)
 *      s(2n+1) = c(n) + c(n+1)
 *
 *  This implementation evaluates c(n) for n ≥ 1 by recursively computing
 *  the pair (c(n), s(n)).  The recursion depth is O(log n) and the total
 *  number of calls is O(n); for n≤10000 this easily satisfies the
 *  timing requirement.  No memoisation is necessary.
 */

#include <stddef.h>

typedef struct { seq_t c; seq_t s; } cs_pair;

/* Recursive computation of (c(n), s(n)) */
static cs_pair cs_rec(idx_t n)
{
    if (n == 1u) {
        cs_pair p = { (seq_t)1, (seq_t)0 };
        return p;
    }

    idx_t k = n >> 1u;
    if ((n & 1u) == 0u) {                 /* n even: c(2k)=s(k), s(2k)=c(k) */
        cs_pair base = cs_rec(k);
        cs_pair ret  = { base.s, base.c };
        return ret;
    } else {                               /* n odd: c(2k+1)=s(k)+s(k+1),
                                            *        s(2k+1)=c(k)+c(k+1) */
        cs_pair p1 = cs_rec(k);
        cs_pair p2 = cs_rec(k + 1u);
        cs_pair ret = { p1.s + p2.s, p1.c + p2.c };
        return ret;
    }
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;
    return cs_rec(n).c;
}

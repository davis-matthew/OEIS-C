/*
 * A347387  The exponent of the first power of 2 reached when starting
 *          iterating A347385 from n, where A347385 is Dedekind psi function
 *          applied to the odd part of n.
 * https://oeis.org/A347387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1435, u16 n <= 9866, u32 n <= 9866, u64 n <= 9866,
 *            u128 n <= 9866, big any n
 * Helpers:   psi_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/psi_odd.h"
/* A347387: a(n) is the exponent k of the first power of two 2^k
 * reached by iterating the Dedekind ψ-function applied to the odd
 * part of n.  The function repeatedly replaces the current value
 * x by ψ(odd(x)) until x becomes a power of two, then returns log2(x). */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t cur = (seq_t)n;

    /* iterate until cur is a power of two */
    while ((cur & (cur - 1)) != 0) {
        cur = psi_odd(cur);
    }

    /* cur is now 2^k; find k */
    seq_t k = 0;
    while (cur > 1) {
        cur >>= 1;
        ++k;
    }
    return k;
}

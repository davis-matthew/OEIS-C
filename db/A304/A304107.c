/*
 * A304107  Analog for squarefree numbers when n is factored in polynomial
 *          ring GF(2)[X], so that the binary expansion of n defines the
 *          corresponding (0,1)-polynomial. These are numbers n such that the
 *          said polynomial doesn't have any duplicated irreducible divisors.
 * https://oeis.org/A304107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 8701, u32 n <= 8701, u64 n <= 8701,
 *            u128 n <= 8701, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"

/* Determine whether the integer m corresponds to a squarefree
   polynomial over GF(2)[X] (i.e., no duplicated irreducible divisors). */
static bool is_squarefree(idx_t m)
{
    seq_t f = (seq_t)m;
    if (f == (seq_t)0)            /* 0 is not in the sequence, but safe here */
        return true;

    /* Derivative over GF(2): keep bits of odd degrees, shifted right one. */
    seq_t d = (seq_t)0;
    unsigned int deg = bitlen_seq_t(f);
    for (unsigned int i = 1; i < deg; i += 2)
        if (f & (((seq_t)1) << i))
            d |= (((seq_t)1) << (i - 1));

    /* Compute GCD(f, d) over GF(2). */
    seq_t a = f;
    seq_t b = d;
    while (b != (seq_t)0) {
        unsigned int deg_a = bitlen_seq_t(a);
        unsigned int deg_b = bitlen_seq_t(b);
        if (deg_a < deg_b) {
            seq_t tmp = a; a = b; b = tmp;
            continue;
        }
        a ^= b << (deg_a - deg_b);
    }
    return a == (seq_t)1;
}

/* Return the n-th term of OEIS A304107 (indices start at 1). */
seq_t a(idx_t n)
{
    if (n == 0U)  /* undefined, but harmless */
        return (seq_t)0;

    idx_t count = 0;
    idx_t m = 0;
    while (count < n) {
        ++m;
        if (is_squarefree(m))
            ++count;
    }
    return (seq_t)m;
}

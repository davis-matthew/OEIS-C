/*
 * A289602  a(n) is the number of permutations of length n that avoid the
 *          pattern 321 and the mesh pattern (21, 257) or the same sequence
 *          for the mesh patterns (21, 261), (21, 273), (21, 277), (21, 321),
 *          (21, 325), (21, 337), (21, 341).
 * https://oeis.org/A289602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n) - coeff of x^n in x^2*(1-x)^2/(1-2x)^2 Which
 *            simplifies to: - a(n) = Catalan(n) for n = 0,1 - a(2) =
 *            Catalan(2) - 1 - a(3) = Catalan(3) - 2 - a(n) = Catalan(n) -
 *            (n+1)*2^(n-4) for n >= 4 (All operations are performed in
 *            seq_t.)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-01)
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/catalan.h"
#include "../utils/pow2.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    seq_t cat = catalan(n);      /* Catalan(n) */
    seq_t sub = (seq_t)0;        /* subtract value */

    if (n >= 4) {
        /* (n+1) * 2^(n-4) */
        sub = mul_small(pow2(n-4), (unsigned int)(n + 1));
    } else if (n == 3) {
        sub = (seq_t)2;
    } else if (n == 2) {
        sub = (seq_t)1;
    }
    /* cat >= sub for all n, but guard against underflow */
    if (cat < sub) return (seq_t)0;
    return cat - sub;
}

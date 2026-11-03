/*
 * A322418  Least k > 0 such that A014221(k) == A014221(k+1) mod n.
 * https://oeis.org/A322418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k>0 such that A014221(k) ≡ A014221(k+1) (mod n)
 * Range:     u8 n <= 31, u16 n <= 31, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 90 (2026-11-01)
 * Helpers:   powmod, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the sequence by evaluating the tower
   2↑↑k mod n.  The sequence stabilises quickly;  we iterate until the
   value no longer changes and return that index.  To compute
   2↑↑k mod n we use a recursive routine that reduces the exponent by
   the totient φ(n), adding φ(n) when the exponent is large.  The
   function `exp_ge_limit` checks whether 2 ↑↑ height ≥ limit using
   simple bounds that are sufficient for n up to 10⁴. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/powmod.h"
#include "../utils/totient_seq.h"

/* Return true if 2 ↑↑ height >= limit.  The function is valid for
   height >= 1 and limit up to a few ten thousands. */
static bool exp_ge_limit(idx_t height, seq_t limit)
{
    if (height == 0)            return (seq_t)1 >= limit;   /* unused */
    if (height == 1)            return (seq_t)2 >= limit;
    if (height == 2)            return (seq_t)4 >= limit;
    if (height == 3)            return (seq_t)16 >= limit;
    if (height == 4)            return (seq_t)65536 >= limit;
    /* For height >= 5, 2 ↑↑ height is astronomically large,
       certainly >= limit (which is at most n <= 10000). */
    return true;
}

/* Compute 2 ↑↑ height (tower of 2's of given height) modulo mod.
   mod == 1 ⇒ result 0.  height >= 1. */
static seq_t tetration_mod(seq_t mod, idx_t height)
{
    if (mod == 1) return 0;
    if (height == 1) return 2 % mod;

    seq_t phi = totient_seq(mod);
    seq_t exp = tetration_mod(phi, height - 1);   /* exponent modulo φ(mod)   */
    if (exp_ge_limit(height - 1, phi))
        exp += phi;          /* large exponent: add φ(mod)  */

    /* exponent fits in uint64_t because φ(mod) <= mod <= n <= 10⁴   */
    return powmod(2, (uint64_t)exp, mod);
}

/* Main sequence function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, behaviour below irrelevant */

    idx_t k = 1;
    seq_t prev = tetration_mod(n, 1);     /* 2 mod n */
    while (1) {
        seq_t cur = tetration_mod(n, k + 1);   /* next term of tower mod n */
        if (cur == prev) return k;
        prev = cur;
        ++k;
    }
}

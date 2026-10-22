/*
 * A316262  Numbers k such that gcd(k, floor(phi*k)) > 1, where phi is the
 *          golden ratio.
 * https://oeis.org/A316262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 23, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th term of OEIS A316262.
   A term is a positive integer k such that gcd(k,⌊ϕ·k⌋) > 1
   (ϕ = (1+√5)/2, the golden ratio) with offset 1.  The sequence
   starts with 4, 6, 8, 10, …  */

#include "../utils/gcd.h"
#include "../utils/floor_sqrt.h"

/* Helper that returns ⌊k·ϕ⌋ using only integer arithmetic.
   ⌊k·ϕ⌋ = ⌊(k + k·√5)/2⌋  and  k·√5 = ⌊√(5·k²)⌋. */
static seq_t floor_phi_mul(idx_t k)
{
    seq_t kk = (seq_t)k;
    seq_t x  = (seq_t)5 * kk * kk;          /* 5·k² */
    seq_t s  = floor_sqrt(x);               /* ⌊k·√5⌋ */
    seq_t sum = kk + s;                     /* k + k·√5 */
    return sum / 2;                         /* ⌊k·ϕ⌋ */
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    idx_t k = 1;      /* current candidate */
    idx_t cnt = 0;    /* number of terms found so far */

    while (1) {
        seq_t kk   = (seq_t)k;
        seq_t phi_k = floor_phi_mul(k);
        if (gcd(kk, phi_k) > 1) {
            ++cnt;
            if (cnt == n)
                return kk;
        }
        ++k;
    }
}

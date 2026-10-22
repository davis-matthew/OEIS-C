/*
 * A315020  Coordination sequence Gal.4.76.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=5, a(2)=9 For n>=3: a(n) = 9 + 38*floor((n-2)/8) +
 *            S[n%8] where S = {0,6,10,14,20,24,29,34} This follows from the
 *            repeating difference pattern: diff(n) = 5 if n%8==0 or 1 6 if
 *            n%8==3 or 6 4 otherwise
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t prefix[9] = {
    0,   // r = 0
    6,   // r = 1
   10,   // r = 2
   14,   // r = 3
   20,   // r = 4
   24,   // r = 5
   29,   // r = 6
   34,   // r = 7
   38    // r = 8 (full cycle sum)
};

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 5;
    if (n == 2) return 9;

    /* n >= 3: use the derived formula */
    idx_t L = n - 2;                 /* number of terms from k=3 to k=n */
    idx_t c = L / 8;                 /* full cycles of 8 terms */
    idx_t r = L % 8;                 /* remaining part of the cycle */

    /* 9 = a(2) because a(2) = 9 */
    seq_t result = (seq_t)9 + (seq_t)c * 38 + prefix[r];
    return result;
}

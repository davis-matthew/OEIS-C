/*
 * A277901  If A010060(n) = 1, a(n) = A065621(A115384(n)), otherwise a(n) =
 *          A048724(a(floor(n/2))).
 * https://oeis.org/A277901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   if A010060(n)=1 then a(n)=A065621(A115384(n)), otherwise
 *            a(n)=A048724(a(floor(n/2)))
 * Range:     u8 n <= 255, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 94 terms through n = 9277 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A277901_par(idx_t x)
{
    int p = 0;
    while (x) { p ^= 1; x &= x - 1; }
    return p;
}

seq_t a(idx_t n)
{
    int k = 0;
    while (!A277901_par(n)) { n >>= 1; k++; }

    /* A115384(n): number of k in 0..n with odd binary weight */
    idx_t c = (n + 1) / 2;
    if ((n & 1) == 0) c += 1;   /* n is odious here, so it counts itself */

    /* A065621(c) */
    seq_t m = (seq_t)c;
    seq_t low = (seq_t)(c & (~c + 1));
    seq_t r = m ^ ((m - low) << 1);

    /* apply A048724 k times */
    while (k-- > 0) r = r ^ (r << 1);
    return r;
}

/*
 * A371594  Starting positions of runs in the paperfolding sequence A014707.
 * https://oeis.org/A371594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n-1 - ((n + A014707(n-1)) mod 2) for n ≥ 2, a(1)=1
 *            where A014707(m) = ((m >> (v2(m)+1)) & 1) and v2(m) counts
 *            trailing zeros in m (A014707(0)=0)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t paperfolding(idx_t x)
{
    if (x == 0) return 0;
    idx_t t = x;
    unsigned trailing = 0;
    while ((t & 1) == 0) {
        ++trailing;
        t >>= 1;
    }
    /* t is odd, shift by trailing+1 */
    if (trailing + 1 >= (sizeof(seq_t) * 8)) {
        return 0;
    }
    return ((seq_t)x >> (trailing + 1)) & 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, harmless */
    if (n == 1) return 1;          /* base case */
    seq_t pf = paperfolding(n - 1);
    seq_t mod = ((seq_t)n + pf) & 1;   /* (n + pf) mod 2 */
    seq_t result = (seq_t)2 * (seq_t)n - 1 - mod;
    return result;
}

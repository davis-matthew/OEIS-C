/*
 * A306556  Integers that appear as (unreduced) numerators of segment
 *          endpoints when a ternary Cantor set is created.
 * https://oeis.org/A306556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(1) = 0 a(2) = 1 a(2^m) = 3^(m-1) (m ≥ 1) a(2^m + k) =
 *            2·3^(m-1) + a(k) (1 ≤ k ≤ 2^m−1)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
    
/* small helper: compute 3^exp for exp ≥ 0 */
static seq_t pow3(unsigned exp)
{
    seq_t r = 1;
    while (exp--)
        r *= 3;
    return r;
}
    
/* recursive definition of a(n) – works for n ≥ 1 */
static seq_t a_rec(idx_t n)
{
    if (n == 1)               /* a(1) = 0 */
        return 0;
    
    /* find m = ⌊log₂(n)⌋ and power = 2^m */
    idx_t t = n;
    int m = 0;
    while (t >> 1) {
        t >>= 1;
        ++m;
    }
    idx_t power = ((idx_t)1) << m;   /* 2^m */
    
    if (power == n)          /* n is a power of 2: a(2^m) = 3^(m-1) */
        return pow3((unsigned)(m - 1));
    
    /* n = 2^m + k with 1 ≤ k < 2^m */
    seq_t base = pow3((unsigned)(m - 1));      /* 3^(m-1)  */
    return 2 * base + a_rec(n - power);        /* a(k) from recursion */
}
    
/* public entry point */
seq_t a(idx_t n)
{
    return a_rec(n);
}

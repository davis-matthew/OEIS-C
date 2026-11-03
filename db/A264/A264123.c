/*
 * A264123  Number of (n+1) X (3+1) arrays of permutations of 0..n*4+3 with
 *          each element having index change +-(.,.) 0,0 1,-2 or 2,-1.
 * https://oeis.org/A264123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=4, a(2)=45, a(3)=576, a(4)=5733, and for n≥5 *
 *            a(n)=53361*10^(n‑5) (equivalently a(n)=10*a(n‑1) for n>5)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 205 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)           /* out of range – behaviour undefined, return 0 */
        return (seq_t)0;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)45;
    if (n == 3) return (seq_t)576;
    if (n == 4) return (seq_t)5733;
    /* n ≥ 5 */
    seq_t p = pow10(n - 5);
    return (seq_t)53361 * p;
}

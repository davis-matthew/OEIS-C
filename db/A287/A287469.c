/*
 * A287469  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 276", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A287469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n odd, a(n) = (10^(n+1) - 10^(n/2)) / 9 for n even
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 115 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The pow10 utility returns seq_t = 10^e */
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
    if (n & 1u)          /* n is odd -> term 0 */
        return (seq_t)0;

    idx_t half = n >> 1;           /* n/2 */
    seq_t p1  = pow10(n + 1);      /* 10^(n+1) */
    seq_t p2  = pow10(half);       /* 10^(n/2) */
    return (p1 - p2) / (seq_t)9;   /* exact division by 9 */
}

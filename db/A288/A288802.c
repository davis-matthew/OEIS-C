/*
 * A288802  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 507", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1) = 11 For odd n ≥ 3: a(n) = 111…1 (n digits) →
 *            repunit(n) For even n: let m = largest power of 2 ≤ (n+2)/2 a(n)
 *            = 111…1 (m digits) followed by (n+1−m) zeros → repunit(m) *
 *            10^{(n+1)-m}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 122 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

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

/* Construct a repunit (digits of all 1’s) of length 'digits'.
   Example: digits = 3 → 111. */
static seq_t repunit(idx_t digits)
{
    seq_t v = 0;
    for (idx_t i = 0; i < digits; ++i) {
        v = mul_small(v, 10);   /* multiply by decimal 10 */
        v = v + 1;              /* add decimal 1 */
    }
    return v;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    /* a(1) is the special case 11 (repunit of length 2) */
    if (n == 1) return repunit(2);

    /* Odd n ≥ 3: all ones of length n */
    if (n & 1) return repunit(n);

    /* Even n: compute m = largest power of 2 ≤ (n+2)/2 */
    idx_t t   = (n + 2) / 2;
    idx_t pow = 1;
    while (pow <= t) pow <<= 1;   /* pow becomes first power > t */
    pow >>= 1;                    /* back to largest power ≤ t */
    idx_t m      = pow;           /* number of leading ones  */
    idx_t shift  = (n + 1) - m;   /* number of trailing zeros */

    seq_t v = repunit(m);          /* leading ones part */
    for (idx_t i = 0; i < shift; ++i) {
        v = mul_small(v, 10);      /* append a zero in decimal */
    }
    return v;
}

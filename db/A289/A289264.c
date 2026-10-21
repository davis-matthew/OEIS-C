/*
 * A289264  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 553", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A289264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A289264 – Binary representation of the diagonal of Rule‑553 CA.
 *  a(0) = 1
 *  If n is odd:      a(n) = (10ⁿ – 1) / 9      // n ones
 *  If n is even (>0):
 *      k  = ⌊log₂ n⌋
 *      Z  = 2^(k-1) – 1
 *      a(n) = 10ⁿ + (10ⁿ⁻ᴢ – 1) / 9           // 1, Z zeros, then n–Z ones
 *
 *  The implementation uses the pow10 utility and an integer floor(log₂).
 */

#include "../utils/pow10.h"

/* integer floor(log₂) for non‑zero unsigned values */
static idx_t floor_log2_idx(idx_t v)
{
    idx_t r = 0;
    while (v > 1) { v >>= 1; ++r; }
    return r;
}

seq_t a(idx_t n)
{
    /* special start value */
    if (n == 0) return (seq_t)1;

    /* odd indices: n ones */
    if (n & 1) {
        seq_t v = pow10(n);
        v = v - (seq_t)1;
        v = v / (seq_t)9;
        return v;
    }

    /* even indices (n ≥ 2) */
    idx_t k   = floor_log2_idx(n);          /* ⌊log₂ n⌋ */
    idx_t Z   = ((idx_t)1 << (k - 1)) - 1;  /* 2^(k-1) – 1 */
    idx_t onesAF = n - Z;                   /* number of trailing ones */

    seq_t v    = pow10(n);                          /* 10ⁿ */
    seq_t rep  = (pow10(onesAF) - (seq_t)1) / (seq_t)9; /* repunit of length onesAF */
    v = v + rep;
    return v;
}

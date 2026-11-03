/*
 * A286864  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 814", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A286864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n = 0 → 1; n = 1 → 10; for n ≥ 2 let k =
 *            floor_log2_uint64(n) – 1, m = 2^k, zeros = (n + 1) – m, then
 *            a(n) = (10^m – 1)/9 × 10^zeros
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 108 (2026-11-01)
 * Helpers:   ilog2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ilog2.h"

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
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)10;

    /* n ≥ 2: compute block size m = 2^{⌊log₂(n)⌋ – 1} */
    idx_t k = ilog2(n) - 1;
    idx_t m = 1ULL << k;                /* m fits in idx_t */

    idx_t zeros = (n + 1) - m;          /* number of trailing zeros */

    /* Build integer 111...1 with m digits       */
    seq_t pow10_m = pow10(m);            /* 10^m */
    seq_t ones    = (pow10_m - 1) / 9;   /* (10^m–1)/9  -> 111…1 */

    /* Append zeros by multiplying with 10^zeros */
    seq_t pow10_z = pow10(zeros);        /* 10^zeros */
    return ones * pow10_z;
}

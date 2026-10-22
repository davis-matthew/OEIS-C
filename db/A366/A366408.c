/*
 * A366408  Starting index in the Thue-Morse sequence (A010060) of the first
 *          maximum length block in which every subword of length n is
 *          distinct.
 * https://oeis.org/A366408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The first maximum–length block in the Thue‑Morse sequence that
 *            contains all distinct subwords of length n starts at position
 *            a(n). For n ≥ 1 the value a(n) is given by the following
 *            piecewise formula (indices are 0‑based): a(1) = 0 Let k = ⌊log₂
 *            n⌋ (i.e. 2ᵏ ≤ n < 2ᵏ⁺¹) and m = n – 2ᵏ (0 ≤ m < 2ᵏ)...
 * Range:     u8 n <= 192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n <= 1u)                  /* a(1) = 0, a(0) is unused */
        return 0;

    /* power‑of‑two indexing */
    const int k = floor_log2_uint64(n);          /* 2ᵏ ≤ n < 2ᵏ⁺¹ */
    const idx_t base = ((idx_t)1) << k;          /* 2ᵏ */
    const idx_t m = n - base;                    /* 0 ≤ m < 2ᵏ */

    /* case n is a power of two (m == 0) */
    if (m == 0u) {
        /* k ≥ 1 because n > 1 */
        const seq_t power = (seq_t)1 << (k - 1); /* 2ᵏ⁻¹ in seq_t */
        return 3 * power + 1;
    }

    /* case n == 2ᵏ + 1 (m == 1) */
    if (m == 1u)
        return 0;

    /* threshold for switching between the two linear regimes */
    const idx_t half = ((idx_t)1) << (k - 1);    /* 2ᵏ⁻¹ */

    if (m <= half) {       /* 2 ≤ m ≤ 2ᵏ⁻¹  */
        const seq_t high = (seq_t)1 << (k + 1); /* 2ᵏ⁺¹  */
        return high + 1 - (seq_t)n;
    } else {               /* 2ᵏ⁻¹ < m < 2ᵏ  */
        const seq_t high = ((seq_t)5) << k;    /* 5·2ᵏ   */
        return high + 1 - (seq_t)n;
    }
}

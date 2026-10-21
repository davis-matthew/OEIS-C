/*
 * A280374  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 260", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A280374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 126, u32 n <= 126, u64 n <= 126,
 *            u128 n <= 126, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A280374 – Decimal representation of the x-axis of Rule 260.
 * Formula: a(n) = Σ_{e∈E} c_e   if e ≤ n and (n – e) ≡ 0 (mod 4),
 * where 
 *    E = {0,1,2,3,5,6,7,8,12}
 *    c_e = {1,1,5,1,4,16,64,256,-256}
 * This follows from the empirical generating function
 *   G(x) = (1 + x + 5x² + x³ + 4x⁵ + 16x⁶ + 64x⁷ + 256x⁸ – 256x¹²)
 *           / ((1 – x)(1 + x)(1 + x²)) = P(x)/(1 – x⁴).
 * As a result the sequence is constant in each 4‑term block after n ≥ 12.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Exponent indices with non‑zero coefficients */
    static const idx_t ev[9] = {0,1,2,3,5,6,7,8,12};
    /* Corresponding coefficients (include a negative value) */
    static const int64_t cv[9] = {1,1,5,1,4,16,64,256,-256};

    int64_t sum = 0;
    for (int i = 0; i < 9; ++i) {
        idx_t e = ev[i];
        if (n >= e && ((n - e) & 3u) == 0u) {
            sum += cv[i];
        }
    }
    return (seq_t)sum;
}

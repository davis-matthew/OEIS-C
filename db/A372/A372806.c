/*
 * A372806  Decimal expansion of 90/781.
 * https://oeis.org/A372806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ⌊10^(n+1) * 90 / 781⌋ mod 10 (decimal digits of 90/781,
 *            a(0) is the first digit after the point)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* All intermediate calculations fit into 64‑bit unsigned. */
    uint64_t rem = 90;          /* current remainder (< 781)   */
    uint64_t digit = 0;         /* digit for the current position */

    for (idx_t i = 0; i <= n; ++i) {
        /* Multiply by 10, obtain the next digit and the new remainder */
        digit = (rem * 10U) / 781U;
        rem   = (rem * 10U) % 781U;
    }

    return (seq_t)digit;
}

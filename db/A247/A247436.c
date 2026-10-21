/*
 * A247436  Base-n state complexity of partitioned deterministic finite
 *          automaton (PDFA) for the periodic sequence (123...14)*.
 * https://oeis.org/A247436
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is periodic with period 14: a(2) = 43, a(3) = 84, a(4) =
 *            43, a(5) = 84, a(6) = 29, a(7) = 15, a(8) = 15, a(9) = 42, a(10)
 *            = 85, a(11) = 42, a(12) = 85, a(13) = 28, a(14) = 15, a(15) =
 *            14, then repeat. Thus a(n) = table[(n-2) mod 14] for n ≥ 2
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle index below offset gracefully */
    if (n < 2) return 0;
    /* Periodic table of values for the 14-cycle */
    static const seq_t tbl[14] = {
        43, 84, 43, 84, 29, 15, 15, 42, 85, 42, 85, 28, 15, 14
    };
    idx_t k = (n - 2) % 14;
    return tbl[k];
}

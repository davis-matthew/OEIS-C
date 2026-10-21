/*
 * A247388  Base-n state complexity of partitioned deterministic finite
 *          automaton (PDFA) for the periodic sequence (12345678)*.
 * https://oeis.org/A247388
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   for n ≥ 8 the value depends only on n mod 8: 0 → 9 1 → 8 2 → 25
 *            3 → 16 4 → 17 5 → 16 6 → 25 7 → 16 For 2 ≤ n ≤ 7 the sequence is
 *            15,16,13,16,23,16 respectively
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special small‑n cases (offset 2) */
    if (n < 8) {
        if (n == 2) return (seq_t)15;
        if (n == 3) return (seq_t)16;
        if (n == 4) return (seq_t)13;
        if (n == 5) return (seq_t)16;
        if (n == 6) return (seq_t)23;
        if (n == 7) return (seq_t)16;
        return (seq_t)0;   /* undefined for n = 0,1 */
    }

    /* General rule for n ≥ 8 */
    static const seq_t tbl[8] = {
        (seq_t)9,  /* n % 8 == 0 */
        (seq_t)8,  /* n % 8 == 1 */
        (seq_t)25, /* n % 8 == 2 */
        (seq_t)16, /* n % 8 == 3 */
        (seq_t)17, /* n % 8 == 4 */
        (seq_t)16, /* n % 8 == 5 */
        (seq_t)25, /* n % 8 == 6 */
        (seq_t)16  /* n % 8 == 7 */
    };
    return tbl[n % 8];
}

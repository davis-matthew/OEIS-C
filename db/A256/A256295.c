/*
 * A256295  Apply the transformation 0 -> 1 -> 2 -> 3 -> 4 -> 0 to the digits
 *          of n written in base 5, then convert back to base 10.
 * https://oeis.org/A256295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   apply the digitwise map 0→1,1→2,2→3,3→4,4→0 to n written in base
 *            5, then interpret the resulting base‑5 number as a decimal
 *            (base‑10). 0 is treated as the single digit 0
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1; /* base‑5 "0" transforms to "1" */
    seq_t result = 0;
    seq_t multiplier = 1;
    while (n > 0) {
        idx_t d  = n % 5;
        idx_t d2 = (d + 1) % 5;
        result += (seq_t)d2 * multiplier;
        multiplier *= (seq_t)5;
        n /= 5;
    }
    return result;
}

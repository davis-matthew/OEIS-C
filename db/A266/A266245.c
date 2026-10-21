/*
 * A266245  Decimal representation of the n-th iteration of the "Rule 9"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n/2 + 90*floor(4^n/60) - 90 for odd n>3; a(n) = 29 for
 *            even n>2 (except a(0)=1,a(1)=0,a(2)=5,a(3)=96). Recurrence: a(n)
 *            = 17*a(n-2) - 16*a(n-4) for n>7
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)96;
    if ((n & 1u) == 0)    /* even n > 3 */
        return (seq_t)29;

    /* odd n >= 5 */
    seq_t pow4 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow4 *= (seq_t)4;            /* 4^n */

    seq_t part1 = pow4 / (seq_t)2;        /* 4^n / 2 */
    seq_t part2 = (seq_t)90 * (pow4 / (seq_t)60);  /* 90 * floor(4^n / 60) */
    return part1 + part2 - (seq_t)90;
}

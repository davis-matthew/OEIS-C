/*
 * A242718  The number of ways that a score of n can be obtained using one
 *          dart on a standard dartboard.
 * https://oeis.org/A242718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 1 if n can be obtained by a single dart on a standard
 *            dartboard (by hitting 1‑20 in a single, double, or triple, or
 *            the bullseye 25 or 50); otherwise a(n) = 0
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 20) return 1;               /* single 1‑20 */
    if (n <= 40) {                       /* double 1‑20 */
        if ((n & 1) == 0) return 1;
    }
    if (n <= 60) {                       /* triple 1‑20 */
        if ((n % 3) == 0) return 1;
    }
    if (n == 25 || n == 50) return 1;    /* outer/inner bull */
    return 0;
}

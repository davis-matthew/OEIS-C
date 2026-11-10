/*
 * A354445  Number of polynomials per row where the minimum number of rows and
 *          polynomials per row necessary to transform A335105 into a
 *          triangular array are present.
 * https://oeis.org/A354445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n >= 2, a(n) = n - A335105_rowlength(n-1), where
 *            the row length of A335105 for element z is 2 * (number of
 *            occupied subshells of z). The subshells are counted by filling
 *            them in Madelung order, with a table of corrections for the
 *            elements whose real electron configurations differ in...
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Validated: standard, pass, 91 terms through n = 101 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* number of occupied subshells of element z under Madelung (Aufbau) filling */
static idx_t A354445_aufbau_count(idx_t z)
{
    idx_t rem = z;
    idx_t count = 0;
    for (int s = 1; rem > 0; ++s) {          /* s = n + l */
        for (int l = s - 1; l >= 0 && rem > 0; --l) {
            int nn = s - l;                   /* increasing n within same n+l */
            if (nn < l + 1) continue;         /* need l <= n-1 */
            idx_t cap = (idx_t)(2 * (2 * l + 1));
            rem -= (rem < cap) ? rem : cap;
            ++count;
        }
    }
    return count;
}

/* real electron configurations that differ from Madelung in subshell count */
static int A354445_correction(idx_t z)
{
    switch (z) {
        case 46: return -1;   /* Pd: 4d10 5s0 */
        case 58:              /* Ce: 4f1 5d1 6s2 */
        case 64:              /* Gd: 4f7 5d1 6s2 */
        case 91:              /* Pa: 5f2 6d1 7s2 */
        case 92:              /* U:  5f3 6d1 7s2 */
        case 93:              /* Np: 5f4 6d1 7s2 */
        case 96:              /* Cm: 5f7 6d1 7s2 */
            return 1;
        default:
            return 0;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    idx_t z = n - 1;
    idx_t cnt = A354445_aufbau_count(z);
    int c = A354445_correction(z);
    if (c < 0) cnt -= (idx_t)(-c); else cnt += (idx_t)c;
    idx_t len = 2 * cnt;
    if (len > n) return (seq_t)0;
    return (seq_t)(n - len);
}

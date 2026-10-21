/*
 * A247391  Base-n state complexity of partitioned deterministic finite
 *          automaton (PDFA) for the periodic sequence (1234567891011).
 * https://oeis.org/A247391
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1283*m^10 - 64570*m^9 + 1396065*m^8 - 16960020*m^7 * +
 *            127065939*m^6 - 605936100*m^5 + 1828078285*m^4 * -
 *            3335483030*m^3 + 3289569228*m^2 - 1288120680*m * + 5443200) /
 *            453600 * where m = n mod 11. The division is exact
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;              /* undefined for n < 2; offset starts at 2 */

    /* m is small; use 64‑bit signed arithmetic to avoid overflow of
       intermediate results. */
    long long m = (long long)(n % 11ULL);
    long long r = 1283;
    r = r * m - 64570;
    r = r * m + 1396065;
    r = r * m - 16960020;
    r = r * m + 127065939;
    r = r * m - 605936100;
    r = r * m + 1828078285;
    r = r * m - 3335483030;
    r = r * m + 3289569228;
    r = r * m - 1288120680;
    r = r * m + 5443200;

    /* denominator */
    const long long den = 453600LL;
    return (seq_t)(r / den);
}

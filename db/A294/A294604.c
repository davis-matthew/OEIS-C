/*
 * A294604  Number of ordinary double points of a family of threefolds.
 * https://oeis.org/A294604
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n % 3 == 0 ? (1/18)*(7*n^4 - 24*n^3 + 39*n^2 - 36*n +
 *            18) : (1/18)*(7*n^4 - 24*n^3 + 37*n^2 - 30*n + 10))
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 325, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 3. For n < 3 the value is
       irrelevant for the tests; we return 0 in that case. */
    if (n < 3) return 0;

    /* Use 128‑bit signed arithmetic to avoid overflow and allow
       intermediate negative values that occur in the polynomial. */
    __int128 n128 = n;
    __int128 n2 = n128 * n128;           /* n^2   */
    __int128 n3 = n2 * n128;             /* n^3   */
    __int128 n4 = n2 * n2;               /* n^4   */
    __int128 num;                        /* numerator of the polynomial */

    if (n % 3 == 0) {
        num  = (__int128)7 * n4;
        num -= (__int128)24 * n3;
        num += (__int128)39 * n2;
        num -= (__int128)36 * n128;
        num += (__int128)18;
    } else {
        num  = (__int128)7 * n4;
        num -= (__int128)24 * n3;
        num += (__int128)37 * n2;
        num -= (__int128)30 * n128;
        num += (__int128)10;
    }

    /* Division by 18 is exact by the OEIS definition. */
    return (seq_t)(num / (__int128)18);
}

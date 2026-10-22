/*
 * A307177  Decimal expansion of smallest nontrivial base-10 number that
 *          contains all pairwise products of its digits as substrings.
 * https://oeis.org/A307177
 *
 * Offset:    37    (a(37) is the first term)
 * Keywords:  nonn, cons, fini, full, base
 * Formula:   a(n) = nth digit (starting from a(37)) of the minimal number
 *            1012014015216242530327281835456364849; returns 0 for indices
 *            outside the valid range
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The minimal number is 1012014015216242530327281835456364849.
       It consists of 37 digits, and the first digit corresponds to a(37). */
    static const char s[] = "1012014015216242530327281835456364849";
    const idx_t len = (idx_t)(sizeof s - 1);   /* 37 */

    if (n < 37) return (seq_t)0;              /* undefined, return 0 */
    idx_t pos = n - 37;
    if (pos >= len) return (seq_t)0;          /* beyond the number's digits */
    return (seq_t)(s[pos] - '0');
}

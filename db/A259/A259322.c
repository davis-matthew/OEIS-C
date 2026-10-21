/*
 * A259322  Sum of sixth powers of odd numbers.
 * https://oeis.org/A259322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(-31+196*n^2-336*n^4+192*n^6)/21
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 17, u64 n <= 412, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate type to avoid overflow while
       the result fits in seq_t for all n whose value fits in
       seq_t.  This works for uint64_t builds and for arbitrary‑
       precision builds that support the builtin unsigned __int128. */
    __uint128_t nn = (unsigned __int128)n;
    __uint128_t n2 = nn * nn;
    __uint128_t n4 = n2 * n2;
    __uint128_t n6 = n4 * n2;

    __uint128_t g = (unsigned __int128)196 * n2
                    - (unsigned __int128)336 * n4
                    + (unsigned __int128)192 * n6
                    - (unsigned __int128)31;

    __uint128_t num = nn * g;

    return (seq_t)(num / (unsigned __int128)21);
}

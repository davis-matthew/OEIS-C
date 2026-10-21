/*
 * A244874  Number of magic labelings with magic sum n of 6th graph shown in
 *          link.
 * https://oeis.org/A244874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-315*(-129+(-1)^n) + 138528*n + 202104*n^2 + 171248*n^3
 *            + 93030*n^4 + 32312*n^5 + 6636*n^6 + 632*n^7) / 40320
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Using unsigned __int128 for intermediate arithmetic to avoid overflow */
    unsigned __int128 un = n;

    unsigned __int128 n2 = un * un;
    unsigned __int128 n3 = n2 * un;
    unsigned __int128 n4 = n3 * un;
    unsigned __int128 n5 = n4 * un;
    unsigned __int128 n6 = n5 * un;
    unsigned __int128 n7 = n6 * un;

    signed __int128 num = 0;
    /* Compute the constant term involving (-1)^n */
    signed __int128 inner = -129 + ((n & 1) ? -1 : 1);
    num += -315 * inner;

    /* Add polynomial terms */
    num += 138528 * (signed __int128)un;
    num += 202104 * (signed __int128)n2;
    num += 171248 * (signed __int128)n3;
    num += 93030  * (signed __int128)n4;
    num += 32312  * (signed __int128)n5;
    num += 6636   * (signed __int128)n6;
    num += 632    * (signed __int128)n7;

    /* Final division by 40320, result is guaranteed to be non‑negative */
    unsigned __int128 res = num / 40320;

    return (seq_t)res;
}

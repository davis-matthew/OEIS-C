/*
 * A270874  a(n) = n^10 + 9*n^9 + 53*n^8 + 218*n^7 + 695*n^6 + 1754*n^5 +
 *          3572*n^4 + 5854*n^3 + 7510*n^2 + 6559*n + 34.
 * https://oeis.org/A270874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^10 + 9*n^9 + 53*n^8 + 218*n^7 + 695*n^6 + 1754*n^5 +
 *            3572*n^4 + 5854*n^3 + 7510*n^2 + 6559*n + 34
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 8, u64 n <= 83, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n1 = (seq_t)n;
    seq_t n2 = n1 * n1;        /* n^2 */
    seq_t n3 = n2 * n1;        /* n^3 */
    seq_t n4 = n3 * n1;        /* n^4 */
    seq_t n5 = n4 * n1;        /* n^5 */
    seq_t n6 = n5 * n1;        /* n^6 */
    seq_t n7 = n6 * n1;        /* n^7 */
    seq_t n8 = n7 * n1;        /* n^8 */
    seq_t n9 = n8 * n1;        /* n^9 */
    seq_t n10 = n9 * n1;       /* n^10 */

    seq_t result = 34;
    result += 6559 * n1;
    result += 7510 * n2;
    result += 5854 * n3;
    result += 3572 * n4;
    result += 1754 * n5;
    result += 695 * n6;
    result += 218 * n7;
    result += 53 * n8;
    result += 9 * n9;
    result += n10;

    return result;
}

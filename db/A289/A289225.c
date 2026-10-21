/*
 * A289225  Number of ways to select 4 disjoint point triples from an n X n X
 *          n triangular point grid, each point triple forming a 2 X 2 X 2
 *          triangle.
 * https://oeis.org/A289225
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (n^8 -8*n^7 -50*n^6 +556*n^5 +231*n^4 -12388*n^3
 *            +17914*n^2 +86648*n -198528)/24 (valid for n >= 5; a(4)=0)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 17, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 4)          /* offset is 4; a(4)=0, other n are undefined */
        return 0;

    seq_t x  = (seq_t)n;
    seq_t n2 = x * x;
    seq_t n3 = n2 * x;
    seq_t n4 = n3 * x;
    seq_t n5 = n4 * x;
    seq_t n6 = n5 * x;
    seq_t n7 = n6 * x;
    seq_t n8 = n7 * x;

    seq_t num = n8
              - (seq_t)8   * n7
              - (seq_t)50  * n6
              + (seq_t)556 * n5
              + (seq_t)231 * n4
              - (seq_t)12388 * n3
              + (seq_t)17914 * n2
              + (seq_t)86648 * x
              - (seq_t)198528;

    return num / (seq_t)24;
}

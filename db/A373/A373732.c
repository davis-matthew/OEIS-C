/*
 * A373732  a(n) = floor(4*n^2/sqrt(3)).
 * https://oeis.org/A373732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(4*n^2 / sqrt(3))
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed reliably using integer arithmetic by finding the largest
   integer a such that 3*a*a <= 16*n^4.  This avoids floating point
   calculations and works for any seq_t size supported by the build. */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic to avoid overflow */
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;          /* n^2 */
    seq_t n4 = n2 * n2;        /* n^4 */
    seq_t limit = 16 * n4;     /* 16 * n^4 */

    /* Upper bound for a.  Since a < 4*n^2 / sqrt(3) < 4*n^2,
       we can safely use 4*n^2 as the search upper limit. */
    seq_t low = 0;
    seq_t high = 4 * n2;        /* safe upper bound */
    seq_t result = 0;

    /* Binary search for maximal a with 3*a*a <= limit */
    while (low <= high) {
        seq_t mid = (low + high) / 2;
        seq_t mid_sq = mid * mid;          /* mid^2   */
        seq_t lhs = 3 * mid_sq;           /* 3*mid^2 */
        if (lhs <= limit) {
            result = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;        /* avoid underflow */
            high = mid - 1;
        }
    }
    return result;
}

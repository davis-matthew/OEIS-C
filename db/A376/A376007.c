/*
 * A376007  For integers n>=4, greatest integer that can satisfy
 *          sqrt((n^2-c)*b^2 + c*(b+1)^2) where b and c are positive integers
 *          and c < n^2.
 * https://oeis.org/A376007
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3-4n+8)/8 if n % 4 == 0 (n^3-9n+24)/16 if n % 8 == 1
 *            or 7 n^3/8 if n % 4 == 2 (n^3-n+8)/16 if n % 8 == 3 or 5 (a(4)
 *            is the first term)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4U) return 0; /* offset behaviour is irrelevant */

    seq_t ns = (seq_t)n;          /* cast to sequence type */
    seq_t mod4 = ns & 3;          /* n % 4 */
    seq_t mod8 = ns & 7;          /* n % 8 */
    seq_t res;

    /* compute n^3 once for reuse */
    seq_t n3 = ns * ns * ns;

    if (mod4 == 0) {
        /* (n^3 - 4n + 8) / 8 */
        res = n3 - (4 * ns) + 8;
        res /= 8;
    } else if (mod4 == 2) {
        /* n^3 / 8 */
        res = n3 / 8;
    } else if (mod8 == 1 || mod8 == 7) {
        /* (n^3 - 9n + 24) / 16 */
        res = n3 - (9 * ns) + 24;
        res /= 16;
    } else { /* mod8 == 3 or 5 */
        /* (n^3 - n + 8) / 16 */
        res = n3 - ns + 8;
        res /= 16;
    }

    return res;
}

/*
 * A398697  a(n) = floor(11^n/4).
 * https://oeis.org/A398697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 11*a(n-1) + 5 + 3*(-1)^n, a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Iteratively evaluate the recurrence.
       For each i from 1 to n:
          if i is odd   -> add 2  (5 + 3*(-1) = 2)
          if i is even -> add 8  (5 + 3*(+1) = 8)   */
    seq_t curr = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (i & 1)        /* odd */
            curr = curr * 11 + 2;
        else              /* even */
            curr = curr * 11 + 8;
    }
    return curr;
}

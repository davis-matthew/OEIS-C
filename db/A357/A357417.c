/*
 * A357417  Row sums of the triangular array A357431.
 * https://oeis.org/A357417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = sum of a chain of n integers.
   For i from n down to 1 we keep the smallest integer m > previous
   such that m ≡ 0 (mod i).  The array of these m values is the
   n-th row of A357431; a(n) is the sum of the row.

   Algorithm:
     cur = 0
     sum = 0
     for i = n down to 1:
         cur = ((cur / i) + 1) * i    // smallest m > cur divisible by i
         sum += cur
   Return sum.
*/
seq_t a(idx_t n)
{
    seq_t cur = 0;
    seq_t sum = 0;
    idx_t i = n;

    while (1) {
        cur = ((cur / i) + 1) * i;
        sum += cur;
        if (i == 1) break;
        --i;
    }
    return sum;
}

/*
 * A303818  Representation of the divisor set of n based on parities of
 *          divisor and complementary divisor.
 * https://oeis.org/A303818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 59, u32 n <= 359, u64 n <= 1679,
 *            u128 n <= 9732, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A303818: a(n) is the decimal number obtained by concatenating, for each divisor
   d ≤ √n, a digit determined by the parity of d and its complementary divisor p = n/d.
   The mapping is:
     - d odd:   p odd -> 1   (odd factor of an odd number)
                p even -> 3
     - d even:  p even -> 2   (even factor of an even number, paired with an even factor)
                p odd -> 4
   This produces the sequence given in the OEIS entry. */
seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            idx_t p = n / d;
            int digit;
            if (d & 1)                /* d is odd */
                digit = (p & 1) ? 1 : 3;
            else                      /* d is even */
                digit = (p & 1) ? 4 : 2;
            result = result * 10 + digit;
        }
    }
    return result;
}

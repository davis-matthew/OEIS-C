/*
 * A245321  Sum of digits of n written in fractional base 6/5.
 * https://oeis.org/A245321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 854, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = sum of digits of n in fractional base 6/5.
   The representation of an integer n in base 6/5 is unique and
   obtained by the recurrence:
   n   →  d0 = n mod 6
   n'  = (n - d0)/6 * 5
   repeat until n' = 0.  The sum of the d0, d1, … gives a(n).  */
seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t sum = 0;

    while (m > 0) {
        seq_t d = m % 6;          /* least–significant digit */
        sum += d;

        /* Next value: n' = (n - d)/6 * 5  (exact division) */
        m = (m - d) / 6 * 5;
    }

    return sum;
}

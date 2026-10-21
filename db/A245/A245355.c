/*
 * A245355  Sum of digits of n written in fractional base 8/5.
 * https://oeis.org/A245355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of the digits of n expressed in base 8/5
 * Range:     u8 n <= 603, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The base‑8/5 representation is obtained by the algorithm:  
   for n>0, let d = n mod 8.  The least significant digit is d.  
   The remaining part is (n - d) / 8 * 5. Repeating this gives all
   digits. Then a(n) is the sum of those digits.  */

/* External linkage function as required by the build. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t cur = (seq_t)n;

    while (cur > 0) {
        seq_t d = cur % 8;            /* lowest digit (0..7) */
        sum += d;
        /* cur is divisible by 8 after subtracting d, so divide then multiply */
        cur = ((cur - d) / 8) * 5;
    }
    return sum;
}

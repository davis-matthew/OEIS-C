/*
 * A284807  Write in base k, complement, reverse. Case k = 8.
 * https://oeis.org/A284807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = reverse(complement(base8(n)))
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Returns the nth term of OEIS A284807 for base k = 8:
   1. Write n in base 8.
   2. Replace every digit d by (7 - d) (the complement with respect
      to the base-1).
   3. Reverse the digit order.
   4. Interpret the reversed digits as a base‑8 number and return its
      value in decimal.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)7;          /* base‑8 "0" → complement "7" → 7 */

    seq_t result = 0;
    idx_t v = n;

    while (v > 0) {
        idx_t digit = v % 8;          /* current base‑8 digit */
        idx_t comp  = 7 - digit;      /* complement of that digit */
        result = result * (seq_t)8 + (seq_t)comp;
        v = v / 8;
    }

    return result;
}

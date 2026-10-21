/*
 * A247018  Numbers of the form 3*z^2 + z + 3 for some integer z.
 * https://oeis.org/A247018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 295, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A247018
   a(n) = 3*z^2 + z + 3
   where z = 0          if n = 1
          z = -(n/2)    if n is even and n > 1
          z = (n-1)/2   if n is odd  (n > 1)
   Index n starts at 1 (offset 1).
   The formula enumerates the values of 3*z^2 + z + 3 for all integers z
   in increasing order: z = 0, –1, +1, –2, +2, … .
*/
seq_t a(idx_t n)
{
    /* Handle the offset 1: a(1) = 3 (z = 0) */
    if (n == 1) {
        return (seq_t)3;
    }

    /* Compute the integer z that corresponds to index n */
    long long z;
    if ((n & 1ULL) == 0) {          /* n even → negative z */
        z = -(long long)(n / 2);
    } else {                        /* n odd  → positive z */
        z = (long long)((n - 1) / 2);
    }

    /* Compute a(n) = 3*z^2 + z + 3 */
    long long val = 3LL * z * z + z + 3LL;
    return (seq_t)val;
}

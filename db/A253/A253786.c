/*
 * A253786  a(3n) = 0, a(3n+1) = 0, a(3n+2) = 1 + a(n+1).
 * https://oeis.org/A253786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=1, a(3*n)=0, a(3*n+1)=0, a(3*n+2)=1 +
 *            a(n+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for indices that are 0 or 1 mod 3 */
    if (n % 3 == 0 || n % 3 == 1)
        return 0;

    /* For indices that are 2 mod 3, count how many times we can
       apply a(3k+2)=1+a(k+1) until the argument is no longer 2 mod 3. */
    seq_t result = 0;
    while (n % 3 == 2) {
        result++;          /* one from the recurrence */
        n = n / 3 + 1;     /* compute the argument for the next level */
    }
    return result;
}

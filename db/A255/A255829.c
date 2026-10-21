/*
 * A255829  a(n) = n for n < 9; a(9n) = a(n); if every 9th term (a(9), a(18),
 *          a(27),...) is deleted, this gives back the original sequence.
 * https://oeis.org/A255829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for 1 ≤ n < 9 a(n) = a(n/9) if n%9 == 0 a(n) = a(n -
 *            floor(n/9)) otherwise
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS sequence A255829 */
seq_t a(idx_t n)
{
    /* Base case: for the first 8 indices */
    if (n < 9) {
        return (seq_t)n;
    }

    /* Reduce the index according to the recurrence */
    idx_t q = n / 9;          /* floor(n/9)   */
    if (n % 9 == 0) {         /* n is a multiple of 9 */
        return a(q);
    } else {                  /* other indices */
        return a(n - q);
    }
}

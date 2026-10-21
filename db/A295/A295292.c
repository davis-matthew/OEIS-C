/*
 * A295292  Sum of the products of the smaller and larger parts of the
 *          partitions of n into two parts with the larger part odd.
 * https://oeis.org/A295292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} i*(n-i)*((n-i)&1)
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the sum of products of the two parts of each partition of n
       into two parts, counting only those partitions where the larger part
       is odd.  The larger part is n-i; it is odd iff (n-i)&1 is true. */
    idx_t limit = n >> 1;           /* floor(n/2) */
    seq_t sum = 0;

    for (idx_t i = 1; i <= limit; ++i) {
        idx_t larger = n - i;
        if (larger & 1) {          /* larger part is odd */
            /* cast operands to seq_t before multiplication to avoid
               unintended 64‑bit overflow and to allow correct
               accumulation in arbitrary‑precision seq_t */
            sum += (seq_t)i * (seq_t)larger;
        }
    }
    return sum;
}

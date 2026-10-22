/*
 * A347840  A surjective map of the positive numbers congruent to 5 modulo 8
 *          (A004770) to the positive numbers congruent to 1, 3, or 7 modulo 8
 *          (A047529).
 * https://oeis.org/A347840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A347840: a(n) = (2*A065883((3*b(n)+1)/2)-1)/3,  b(n)=5+8*(n-1)  */
/* Equivalent implementation: let u = 3*n-1; remove all factors of 4 from u;  */
/* then a(n) = (2*u-1)/3.                                                       */

seq_t a(idx_t n)
{
    /* Guard against n < 1 – the sequence is undefined below the offset. */
    if (n == 0) return 0;

    /* Compute u = 3*n - 1 as a seq_t value. */
    seq_t u = (seq_t)n * 3 - 1;

    /* Remove all factors of 4 from u. */
    while ((u & 3u) == 0) {
        u >>= 2;
    }

    /* Compute (2*u - 1) / 3. All operations are performed in seq_t. */
    seq_t tmp = u * 2 - 1;
    return tmp / 3;
}

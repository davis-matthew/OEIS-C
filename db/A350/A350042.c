/*
 * A350042  Sum of all the parts in the partitions of n into 3 positive
 *          integer parts.
 * https://oeis.org/A350042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * ((n*n + 3)/12)
 * Range:     u8 n <= 14, u16 n <= 92, u32 n <= 3721, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from a(n) = n * A069905(n), where A069905(n) = floor((n^2 + 3)/12). */
/* The expression is valid for all non‑negative integer n. */

seq_t a(idx_t n)
{
    /* Promote n to seq_t before any arithmetic to avoid truncation. */
    seq_t N = (seq_t)n;
    seq_t t = N * N;   /* n^2 */
    t += 3;            /* n^2 + 3 */
    t /= 12;           /* floor((n^2 + 3)/12) = number of partitions into 3 parts */
    return N * t;      /* n times the partition count */
}

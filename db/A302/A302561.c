/*
 * A302561  Partial sums of A092182.
 * https://oeis.org/A302561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(12 + n - 64*n^2 + 5*n^3 + 58*n^4) / 12
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 61, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the closed‑form expression for the partial sums of A092182. */

seq_t a(idx_t n)
{
    /* For the defined offset the sequence is only required for n ≥ 1. */
    if (n == 0) return 0;

    /* Use an extended signed integer type for the intermediate arithmetic. */
    __int128 N  = n;          /* n               */
    __int128 n2 = N * N;      /* n^2             */
    __int128 n3 = n2 * N;     /* n^3             */
    __int128 n4 = n3 * N;     /* n^4             */

    /* Compute the polynomial inside the parentheses. */
    __int128 expr = 12
                    + N
                    - (__int128)64 * n2
                    + (__int128)5 * n3
                    + (__int128)58 * n4;

    /* Multiply by n and divide by 12.  All divisions are exact. */
    __int128 num = N * expr;
    num /= 12;

    return (seq_t)num;
}

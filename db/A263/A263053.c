/*
 * A263053  Number of (n+1) X 2 0..1 arrays with each row and column not
 *          divisible by 3, read as a binary number with top and left being
 *          the most significant bits.
 * https://oeis.org/A263053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + 4*a(n-2) - 4*a(n-3)
 * with initial values: a(1)=2, a(2)=2, a(3)=10
 * This recurrence is exact and works for any seq_t supporting
 * addition, subtraction, and left shift by 2 bits (no overflow
 * concerns for built‑in integer types up to the range that
 * fits seq_t, while arbitrary‑precision types naturally grow).
 */

seq_t a(idx_t n)
{
    /* Offset of the sequence is 1; return 0 for n==0 (undefined) */
    if (n == 0)
        return 0;

    /* Small n: return precomputed values directly */
    if (n == 1 || n == 2)
        return 2;
    if (n == 3)
        return 10;

    /* Iterate using the recurrence */
    seq_t a_nm3 = 2;   /* a(n-3) */
    seq_t a_nm2 = 2;   /* a(n-2) */
    seq_t a_nm1 = 10;  /* a(n-1) */
    seq_t a_n   = 0;   /* current term */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t diff = a_nm2 - a_nm3; /* a(n-2) - a(n-3) */
        a_n = a_nm1 + (diff << 2); /* a(n-1) + 4*(a(n-2)-a(n-3)) */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}

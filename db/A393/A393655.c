/*
 * A393655  Total number of descents over all paths from a fixed vertex on
 *          level 2*(n+1) to level 1 in the 3-Bratteli diagram, a(n) =
 *          3^(n-3)*(16*n-13), n>=3.
 * https://oeis.org/A393655
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-3) * (16*n-13) for n >= 3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow3_local(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 3;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;                /* offset is 3; behaviour below 3 is irrelevant */

    seq_t factor  = pow3_local(n - 3);   /* 3^(n-3) */
    seq_t coeff   = (seq_t)16 * n - (seq_t)13;  /* (16*n - 13) as seq_t */
    return factor * coeff;
}

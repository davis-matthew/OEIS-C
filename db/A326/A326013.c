/*
 * A326013  a(n) = (n+1) * (3^n + 1)^n.
 * https://oeis.org/A326013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) * (3^n + 1)^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_custom(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;     /* seq_t multiplication is defined */
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow3  = pow_seq_custom((seq_t)3, n);      /* 3^n          */
    seq_t base  = pow3 + (seq_t)1;                  /* 3^n + 1      */
    seq_t powB  = pow_seq_custom(base, n);          /* (3^n + 1)^n  */
    return ((seq_t)n + (seq_t)1) * powB;            /* (n+1)*(3^n+1)^n */
}

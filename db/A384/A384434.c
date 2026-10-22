/*
 * A384434  Consecutive states of the linear congruential pseudo-random number
 *          generator for CUPL when started at 1.
 * https://oeis.org/A384434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; for n>1, a(n) = (a(n-1) * 452807053) mod 2^31
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)                /* undefined for n<1; return 0 as safe fallback */
        return 0;

    const seq_t multiplier = 452807053ULL;   /* 5^15 mod 2^31 */
    const seq_t mask = 2147483647ULL;        /* 2^31 - 1, mask for modulus */

    seq_t res = 1;                       /* a(1) */
    for (idx_t i = 1; i < n; ++i)        /* iterate n-1 steps */
        res = (res * multiplier) & mask; /* multiply and mask to 31 bits */

    return res;
}

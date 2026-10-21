/*
 * A267596  Decimal representation of the n-th iteration of the "Rule 173"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(n)=4^n-1 for n>=2 * The implementation uses
 *            binary exponentiation in type seq_t. * All arithmetic is
 *            performed with the portable operations * defined for seq_t,
 *            thereby avoiding overflow issues in * builds that use
 *            arbitrary-precision integers. For builds * with a 64‑bit seq_t
 *            t...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)1;
    if (n == 1U)
        return (seq_t)2;

    /* Compute 4^n via exponentiation by squaring. */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)4;
    idx_t exp    = n;

    while (exp != 0U) {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }

    /* Subtract 1 to obtain 4^n - 1. */
    result -= (seq_t)1;
    return result;
}

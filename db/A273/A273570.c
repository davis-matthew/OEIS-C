/*
 * A273570  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 798", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*4^n - 6*2^n + 13 for n > 1 Initial values: a(0) = 1,
 *            a(1) = 5 Computation uses binary exponentiation on seq_t
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    /* compute 2^n */
    seq_t pow2_n = pow_uint((seq_t)2, n);

    /* compute 4^n as (2^n)^2 */
    seq_t pow4_n = pow2_n * pow2_n;

    /* a(n) = 2*4^n - 6*2^n + 13 */
    seq_t res = pow4_n * 2;
    res -= pow2_n * 6;
    res += 13;

    return res;
}

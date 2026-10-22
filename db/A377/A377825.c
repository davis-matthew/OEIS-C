/*
 * A377825  Number of distinct permutations of the terms of the n-th row of
 *          Pascal's triangle with alternating signs.
 * https://oeis.org/A377825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)! / 2^((n*(1+(-1)^n))/4)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: multiply 1…n+1, dividing by 2 as soon as possible to avoid overflow. */
seq_t a(idx_t n)
{
    /* exponent of 2 in the denominator: n/2 if n even, 0 otherwise */
    idx_t exp = ((n & 1ULL) == 0) ? n / 2 : 0;
    seq_t result = 1;

    /* multiply successive terms and reduce by powers of 2 immediately */
    for (idx_t i = 2; i <= n + 1; ++i) {
        result *= (seq_t)i;
        while (exp > 0 && (result & (seq_t)1) == 0) {
            result /= 2;
            --exp;
        }
    }

    /* any remaining powers of 2 (should not happen for the exact range,
       but this guarantees correctness for all n) */
    while (exp > 0) {
        result /= 2;
        --exp;
    }

    return result;
}

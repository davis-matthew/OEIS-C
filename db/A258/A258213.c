/*
 * A258213  Number of permutations of {1,2,3,...,n} such that no even numbers
 *          are adjacent.
 * https://oeis.org/A258213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = factorial(ceil(n/2))*fallfac(ceil(n/2)+1, floor(n/2))
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t half   = (n + 1) / 2;    /* ceil(n/2)   */
    idx_t even   = n / 2;          /* floor(n/2)  */
    seq_t res    = 1;

    /* factorial(half) */
    for (idx_t i = 2; i <= half; ++i)
        res *= (seq_t)i;

    /* falling factorial from half+1 of length even */
    seq_t start = (seq_t)(half + 1);
    for (idx_t i = 0; i < even; ++i)
        res *= (start - (seq_t)i);

    return res;
}

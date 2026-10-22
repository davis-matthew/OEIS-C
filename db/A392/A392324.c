/*
 * A392324  Minimal value of p(1) + Sum_{i=2...n} p(i-1)*p(i), as p ranges
 *          over all permutations of odd numbers {1,3,...,2*n-1}.
 * https://oeis.org/A392324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = ((n-2)*(2*n^2-2*n+9))/3 + 4*floor(n/2) for n>=2; a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    seq_t nn   = (seq_t)n;          /* cast index to seq_t */
    seq_t nm2  = nn - (seq_t)2;     /* n - 2 */
    seq_t n2   = nn * nn;           /* n^2 */
    seq_t termB = (seq_t)2 * n2 - (seq_t)2 * nn + (seq_t)9; /* 2*n^2 - 2*n + 9 */
    seq_t prod = nm2 * termB;       /* (n-2)*(2*n^2-2*n+9) */
    seq_t part1 = prod / (seq_t)3;  /* /3 */
    seq_t part2 = (seq_t)4 * (nn / (seq_t)2); /* 4*floor(n/2) */
    return part1 + part2;
}

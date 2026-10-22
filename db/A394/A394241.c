/*
 * A394241  Minimal value of Sum_{i=1..n-1} p(i)*p(i+1), as p ranges over all
 *          permutations of odd numbers {1,3,...,2*n-1}.
 * https://oeis.org/A394241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (2*n^3 - 6*n^2 + 13*n - (n%2?15:9)) / 3 for n >= 2 a(0) =
 *            0, a(1) = 0
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is defined to be 0 */
    if (n < 2) {
        return 0;
    }

    /* Work in seq_t to avoid overflow on intermediate results */
    seq_t sn   = (seq_t)n;          /* n as seq_t */
    seq_t n2   = sn * sn;           /* n^2 */
    seq_t n3   = n2 * sn;           /* n^3 */

    /* Numerator: 2*n^3 - 6*n^2 + 13*n - (n%2?15:9) */
    seq_t num  = 2 * n3;            /* 2*n^3 */
    num -= 6 * n2;                  /* - 6*n^2 */
    num += 13 * sn;                 /* + 13*n */
    num -= (n & 1) ? 15 : 9;        /* subtract 15 if n odd, else 9 */

    /* Divide by 3 (the numerator is guaranteed divisible by 3) */
    return num / 3;
}

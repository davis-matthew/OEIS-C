/*
 * A340169  a(n) is the number of strings of length n over the alphabet
 *          {a,b,c} with the number of a's divisible by 3, and the number of
 *          b's and c's is at most 3.
 * https://oeis.org/A340169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if n % 3 == 0: 1 + (8/3)*n - 4*n^2 + (4/3)*n^3 if n % 3
 *            == 1: 2*n if n % 3 == 2: -2*n + 2*n^2
 * Range:     u8 n <= 8, u16 n <= 38, u32 n <= 1478, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    switch (n % 3)
    {
        case 0:
        {
            seq_t k = n_t / 3;
            /* a = 1 + 8k - 36k^2 + 36k^3 */
            seq_t result = 1;
            result += 8 * k;
            result -= 36 * k * k;
            result += 36 * k * k * k;
            return result;
        }
        case 1:
            return 2 * n_t;
        case 2:
        {
            /* a = 2*n^2 - 2*n */
            seq_t n2 = n_t * n_t;
            seq_t result = 2 * n2;
            result -= 2 * n_t;
            return result;
        }
    }
    /* unreachable, but for completeness */
    return 0;
}

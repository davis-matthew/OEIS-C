/*
 * A322171  Expansion of x*(3 + 5*x + x^2 + x^3)/((1 - x)^2*(1 + x^2)).
 * https://oeis.org/A322171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n - 2*sin(Pi*n/2) - cos(Pi*n/2)
 * Range:     u8 n <= 50, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)5 * (seq_t)n;
    switch (n & 3u) {
        case 0:             /* n ≡ 0 (mod 4): sin = 0, cos = 1  */
            val -= 1;       /* val = 5*n - 1                   */
            break;
        case 1:             /* n ≡ 1 (mod 4): sin = 1, cos = 0  */
            val -= 2;       /* val = 5*n - 2                   */
            break;
        case 2:             /* n ≡ 2 (mod 4): sin = 0, cos = -1 */
            val += 1;       /* val = 5*n + 1                   */
            break;
        case 3:             /* n ≡ 3 (mod 4): sin = -1, cos = 0 */
            val += 2;       /* val = 5*n + 2                   */
            break;
    }
    return val;
}

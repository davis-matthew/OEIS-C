/*
 * A324400  Lexicographically earliest sequence such that a(i) = a(j) => f(i)
 *          = f(j) for all i, j >= 1, where f(n) = -1 if n = 2^k and k > 0,
 *          and f(n) = n for all other numbers.
 * https://oeis.org/A324400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n <= 3 ? n * : ( (n & (n-1)) == 0 ? 2 * : 1 + n -
 *            floor_log2(n) ) * where floor_log2(n) is the largest k such that
 *            2^k ≤ n
 * Range:     u8 n <= 262, u16 n <= 9975, u32 n <= 9975, u64 n <= 9975,
 *            u128 n <= 9975, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n < 4)
        return (seq_t)n;

    /* Powers of two (except 1) map to 2. */
    if ((n & (n - 1)) == 0)   /* n is a power of two */
        return (seq_t)2;

    /* Compute floor(log2(n)) by shifting. */
    idx_t temp = n;
    idx_t log2 = 0;
    while (temp >>= 1)
        ++log2;

    /* Apply the formula for composite or odd indices. */
    return (seq_t)(1 + n - log2);
}

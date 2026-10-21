/*
 * A286098  Square array read by antidiagonals: A(n,k) = T(n AND k, n OR k),
 *          where T(n,k) is sequence A001477 considered as a two-dimensional
 *          table, AND is bitwise-and (A004198) and OR is bitwise-or
 *          (A003986).
 * https://oeis.org/A286098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the value at index n of the square array read by
 *            antidiagonals, where A(i,j) = T(i & j, i | j), and the pairing
 *            function T(x,y) = (x+y)(x+y+1)/2 + (x & y). This implements
 *            exactly that definition
 * Range:     u8 n <= 255, u16 n <= 8573, u32 n <= 8573, u64 n <= 8573,
 *            u128 n <= 8573, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find the diagonal d such that (d+1)(d+2)/2 > n */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 <= n) {
        ++d;
    }
    /* number of elements before diagonal d */
    idx_t prev = (d * (d + 1)) / 2;
    /* offset within diagonal d */
    idx_t i = n - prev;
    idx_t j = d - i;

    idx_t x = i & j;
    idx_t y = i | j;

    seq_t s = (seq_t)(x + y);
    seq_t t = s * (s + 1) / 2;
    seq_t result = t + (seq_t)(x & y);

    return result;
}

/*
 * A334039  For any n > 0: start with x = n; for k = 1..n, if k divides x then
 *          divide x by k; a(n) corresponds to the final value of x.
 * https://oeis.org/A334039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = final value after starting with x = n and for k = 1..n,
 *            if k divides x then x /= k. a(1)=1 and in general a(n) is the
 *            value that remains after all such divisions
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t ks = (seq_t)k;
        if (x % ks == 0)
            x /= ks;
    }
    return x;
}

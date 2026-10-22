/*
 * A352450  2^k appears in the binary expansion of a(n) iff 2^k appears in the
 *          binary expansion of n and k AND n > 0 (where AND denotes the
 *          bitwise AND operator).
 * https://oeis.org/A352450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum over k where 2^k is set in n and (k & n) != 0 of 2^k
 *            (i.e. a(n) <= n with equality iff n = 0)
 * Range:     u8 n <= 263, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t res = 0;
    idx_t k = 0;
    idx_t t = n;
    while (t) {
        if (t & (idx_t)1) {
            if ((k & n) != 0)
                res |= ((seq_t)1 << k);
        }
        t >>= 1;
        ++k;
    }
    return res;
}

/*
 * A329720  a(n) = Sum_{k=0..n} ((binomial(n-k,6k)*binomial(n,k)) mod 2).
 * https://oeis.org/A329720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n}((binomial(n-k,6k)*binomial(n,k)) mod 2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t sub = n; ; sub = (sub - 1) & n) {
        idx_t k = sub;
        idx_t m = n - k;
        idx_t t = 6 * k;
        if ((t & ~m) == 0)
            res += 1;
        if (sub == 0)
            break;
    }
    return res;
}

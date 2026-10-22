/*
 * A338117  Number of partitions of n into two parts (s,t) such that (t-s) |
 *          n, where s < t.
 * https://oeis.org/A338117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of unordered partitions (s,t) of n (s<t) such that
 *            * (t-s) divides n. Equivalently: * a(n) = divisors(n) – 1 if n
 *            is odd * a(n) = divisors(n) – divisors(odd_part(n)) – 1
 *            otherwise * where odd_part(n) = n with all factors of 2 removed
 * Range:     u8 n <= 9601, u16 n <= 9601, u32 n <= 9601, u64 n <= 9601,
 *            u128 n <= 9601, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts the number of positive divisors of an integer n.
 * This routine uses trial division and works for any n that fits in idx_t. */
static seq_t divisor_count_u64(idx_t n)
{
    seq_t res = 1;
    idx_t t = n;
    /* factor 2 as much as possible */
    int cnt = 0;
    while ((t & 1ULL) == 0) {
        t >>= 1;
        ++cnt;
    }
    if (cnt > 0)
        res = res * (seq_t)(cnt + 1);
    /* trial division by odd numbers */
    for (idx_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            cnt = 0;
            while (t % p == 0) {
                t /= p;
                ++cnt;
            }
            res = res * (seq_t)(cnt + 1);
        }
    }
    if (t > 1)          /* t is a prime larger than sqrt(original n) */
        res = res * 2;
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t d_all = divisor_count_u64(n);

    /* If n is odd, all divisors are odd, so the answer is d_all - 1. */
    if ((n & 1ULL) != 0)
        return d_all - 1;

    /* For even n: remove the factor of 2 to get the odd part m. */
    idx_t m = n;
    while ((m & 1ULL) == 0)
        m >>= 1;
    seq_t d_odd = divisor_count_u64(m);

    /* Even divisors count = d_all - d_odd.  Exclude the divisor n itself. */
    return d_all - d_odd - 1;
}

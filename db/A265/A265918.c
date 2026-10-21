/*
 * A265918  a(n) = A070939(n) mod A000120(n), where A070939(n) is the binary
 *          length of n and A000120(n) is the binary weight of n.
 * https://oeis.org/A265918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A070939(n) mod A000120(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A070939(n) = binary length of n (floor(log2(n)) + 1) */
/* A000120(n) = binary weight of n (number of 1-bits) */

seq_t a(idx_t n)
{
    /* compute the binary length of n */
    idx_t len = 0;
    idx_t t = n;
    while (t)
    {
        t >>= 1;
        ++len;
    }

    /* compute the popcount of n */
    idx_t pop = 0;
    t = n;
    while (t)
    {
        t &= t - 1;   /* clear the lowest set bit */
        ++pop;
    }

    /* pop > 0 for n >= 1, so modulo is safe */
    return (seq_t)len % (seq_t)pop;
}

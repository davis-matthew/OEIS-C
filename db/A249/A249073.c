/*
 * A249073  Ordered union of the sets {h^6, h >=1} and {2*k^6, k >=1}.
 * https://oeis.org/A249073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth smallest of the set {h^6, 2*k^6} (h,k≥1)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 74, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow6(seq_t x)
{
    seq_t r = 1;
    r *= x;
    r *= x;
    r *= x;
    r *= x;
    r *= x;
    r *= x;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not needed for OEIS offset */
    idx_t h = 1, k = 1;
    seq_t val1 = 1;                /* h^6   */
    seq_t val2 = 2;                /* 2*k^6 */
    idx_t count = 0;

    while (count < n)
    {
        if (val1 <= val2)
        {
            ++count;
            if (count == n) return val1;
            ++h;
            val1 = pow6((seq_t)h);
        }
        else
        {
            ++count;
            if (count == n) return val2;
            ++k;
            val2 = pow6((seq_t)k) * (seq_t)2;
        }
    }
    return 0;  /* unreachable */
}

/*
 * A297777  Number of distinct runs in base-9 digits of n.
 * https://oeis.org/A297777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of distinct digits in base‑9 representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1; n>=1 in actual use */

    unsigned mask = 0;
    idx_t x = n;
    while (x != 0) {
        unsigned d = (unsigned)(x % 9u);
        mask |= 1u << d;
        x /= 9u;
    }

    seq_t cnt = 0;
    unsigned t = mask;
    while (t) {
        cnt++;
        t &= t - 1;          /* pop lowest set bit */
    }
    return cnt;
}

/*
 * A349552  a(n) is the number of halving partitions of n (see Comments for
 *          definition).
 * https://oeis.org/A349552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of halving partitions of n (recursive)
 * Range:     u8 n <= 5290, u16 n <= 32768, u32 n <= 32768, u64 n <= 32768,
 *            u128 n <= 32768, big any n
 * Validated: standard, pass, 117 terms through n = 9738 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t count_halving(idx_t rem, idx_t curr)
{
    if (rem == 0) return (seq_t)1;
    if (curr == 1) return (seq_t)1;   /* only 1s can follow */

    idx_t fl = curr / 2;
    idx_t ce = (curr + 1) / 2;
    seq_t res = 0;

    if (fl > 0 && fl <= rem)          /* floor candidate */
        res += count_halving(rem - fl, fl);

    if (ce > 0 && ce <= rem && ce != fl)  /* ceiling candidate, avoid double‑count if equal */
        res += count_halving(rem - ce, ce);

    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t fl = n / 2;
    idx_t ce = (n + 1) / 2;
    seq_t total = 0;

    if (fl > 0 && fl <= n)
        total += count_halving(n - fl, fl);

    if (ce > 0 && ce <= n && ce != fl)
        total += count_halving(n - ce, ce);

    return total;
}

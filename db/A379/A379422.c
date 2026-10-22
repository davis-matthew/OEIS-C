/*
 * A379422  a(n) = n + floor(n*r/t) + floor(n*s/t), where r = 5^(1/4), s =
 *          5^(1/2), t = 5^(3/4).
 * https://oeis.org/A379422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/r⌋ + ⌊n/r²⌋, r = 5^(1/4)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the two floor terms using only integer arithmetic. */

static inline seq_t seq_isqrt(seq_t v)
{
    seq_t lo = 0, hi = v;
    while (lo < hi) {
        seq_t mid = (lo + hi + 1) / 2;
        if (mid * mid <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1: a(0) is unused */

    seq_t nn  = (seq_t)n;
    seq_t nsq = nn * nn;                 /* n²   */
    seq_t n4  = nsq * nsq;                /* n⁴   */

    /* ⌊n/√5⌋ : use inequality k²*5 ≤ n²  → k = ⌊√(n²/5)⌋ */
    seq_t part_sqrt5 = seq_isqrt(nsq / 5);

    /* ⌊n/r⌋ : use inequality 5·k⁴ ≤ n⁴  → k = ⌊4throot(n⁴/5)⌋ */
    seq_t limit = n4 / 5;
    seq_t lo = 0, hi = nn;
    while (lo < hi) {
        seq_t mid = (lo + hi + 1) / 2;
        seq_t mid2 = mid * mid;
        seq_t mid4 = mid2 * mid2;
        if (mid4 <= limit)
            lo = mid;
        else
            hi = mid - 1;
    }
    seq_t part_r = lo;

    return nn + part_sqrt5 + part_r;
}

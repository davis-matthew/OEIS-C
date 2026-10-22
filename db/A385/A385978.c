/*
 * A385978  Number of trailing zeros in (10*n)^(10*n).
 * https://oeis.org/A385978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   a(n) = 10*n*(min(v_2(n), v_5(n))+1)
 * Range:     u8 n <= 19, u16 n <= 1999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute v_2(n) and v_5(n) */
    idx_t t = n;
    int v2 = 0, v5 = 0;
    while ((t & 1ULL) == 0) {
        v2++;
        t >>= 1;
    }
    while (t % 5ULL == 0) {
        v5++;
        t /= 5;
    }

    int m = v2 < v5 ? v2 : v5;                /* min(v2, v5) */
    seq_t mult = (seq_t)m + (seq_t)1;          /* (min+1) */
    seq_t res   = (seq_t)n * ((seq_t)10 * mult);/* 10*n*(min+1) */
    return res;
}

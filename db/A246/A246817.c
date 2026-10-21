/*
 * A246817  Possible number of trailing zeros in hyperfactorials (A002109).
 * https://oeis.org/A246817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   a(n) = A246839(5*(n-1)), where A246839(m) = Σ_{k=1}^{m} k·v_5(k)
 * Range:     u8 n <= 10, u16 n <= 145, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * 5 - 5;   /* m = 5*(n-1) */
    if (m == 0)
        return 0;
    idx_t limit = (idx_t)m;               /* assume m fits into idx_t */
    seq_t sum = 0;
    idx_t power = 5;
    while (power <= limit)
    {
        idx_t t = limit / power;
        seq_t s = (seq_t)t * ((seq_t)t + 1) / 2;   /* Σ_{i=1}^{t} i */
        sum += (seq_t)power * s;                  /* Σ over k multiple of power */
        if (power > limit / 5)
            break;
        power *= 5;
    }
    return sum;
}

/*
 * A245537  Write n>=1 as either n=2^k-2^r with 0 <= r <= k-1, in which case
 *          a(2^k-2^r)=A083424(k-r-1), or as n=2^k-2^r+j with 2 <= r <= k-1, 1
 *          <= j < 2^r-1, in which case a(2^k-2^r+j)=A083424(k-r-1)*a(j).
 * https://oeis.org/A245537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(2^k-2^r) = A083424(k-r-1); a(2^k-2^r+j) = A083424(k-r-1)*a(j)
 *            for 2<=r<=k-1, 1<=j<2^r-1; A083424(m) = (5*4^m+(-2)^m)/6
 * Range:     u8 n <= 62, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 79 terms through n = 79 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A083424_val(idx_t m)
{
    /* A083424(0)=1, (1)=3, (m)=2*(m-1)+8*(m-2) */
    seq_t p = (seq_t)1, q = (seq_t)3;
    if (m == 0) return p;
    for (idx_t i = 2; i <= m; ++i) {
        seq_t t = p * (seq_t)8 + q * (seq_t)2;
        p = q;
        q = t;
    }
    return q;
}

static idx_t A245537_flog2(idx_t x)
{
    idx_t e = 0;
    while (x > 1) {
        x >>= 1;
        ++e;
    }
    return e;
}

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    while (1) {
        idx_t k = A245537_flog2(n) + 1;
        idx_t delta = (((idx_t)1) << k) - n;
        if ((delta & (delta - 1)) == 0) {
            idx_t r = A245537_flog2(delta);
            return result * A083424_val(k - r - 1);
        } else {
            idx_t r = A245537_flog2(delta) + 1;
            idx_t j = n - ((((idx_t)1) << k) - (((idx_t)1) << r));
            result = result * A083424_val(k - r - 1);
            n = j;
        }
    }
}

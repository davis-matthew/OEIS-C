/*
 * A365316  a(n) = number of visible diamonds (squares rotated by 45 degrees)
 *          with diagonal 2 in the large square bounded by the parabola y=x^2
 *          and starting from the point (0, 2n).
 * https://oeis.org/A365316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = a(n-1) + size(n)^2 - (size(n-1)-1)^2, n ≥ 1
 *            size(k) = floor((1 + sqrt(1 + 8*k)) / 2)
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: integer square‑root for an unsigned 64‑bit integer.
   Uses binary search with division to avoid overflow.          */
static idx_t floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = (idx_t)1 << 32; /* hi > sqrt(2^64-1) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > x / mid)  /* mid*mid would overflow or exceed x */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    seq_t a_val     = (seq_t)1;  /* current a value */
    seq_t size_prev = (seq_t)1;  /* size(0) = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        /* Compute size(i) = floor((1 + sqrt(1 + 8*i)) / 2) */
        idx_t val = (idx_t)1 + (idx_t)8 * i;          /* 1 + 8*i  */
        idx_t root = floor_sqrt_u64(val);             /* integer sqrt */
        seq_t size_cur = ((seq_t)1 + (seq_t)root) / 2;

        /* f(i) = size_cur^2 - (size_prev-1)^2 */
        seq_t term = size_cur * size_cur;
        seq_t spm1 = size_prev - (seq_t)1;   /* size_prev - 1  */
        term -= spm1 * spm1;

        a_val += term;
        size_prev = size_cur;
    }

    return a_val;
}

/*
 * A374447  Inverse permutation to A373498.
 * https://oeis.org/A374447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = (r-1)*(2*r-1) + P(r,k) where r is the smallest integer
 *            with r*(2*r+1) ≥ n, k = n - (r-1)*(2*(r-1)+1), and P is defined
 *            by if k ≤ 2r and k odd : 4r - k - 1 if k ≤ 2r and k even : k - 1
 *            if k > 2r and k odd : k if k > 2r and k even : k - 2r This
 *            implements OEIS sequence A374447
 * Range:     u8 n <= 253, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* locate block (row) r */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t val = mid * (2 * mid + 1);
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* offset inside the block */
    idx_t prev = (r - 1) * (2 * (r - 1) + 1);
    idx_t k = n - prev;   /* 1‑based within row */

    /* compute P(r,k) */
    idx_t two_r = 2 * r;
    seq_t P;
    if (k <= two_r) {
        if (k & 1U)        /* odd */
            P = (seq_t)(4 * r - k - 1);
        else               /* even */
            P = (seq_t)(k - 1);
    } else {
        if (k & 1U)        /* odd */
            P = (seq_t)k;
        else               /* even */
            P = (seq_t)(k - 2 * r);
    }

    /* base value for the block */
    seq_t base = (seq_t)(r - 1) * (seq_t)(2 * r - 1);

    return base + P;
}

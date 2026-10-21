/*
 * A287028  {0->101, 1->010}-transform of the Sturmian word A080764.
 * https://oeis.org/A287028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 - A286990(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_u64(idx_t n)
{
    /* returns floor(sqrt(n)) for non‑negative n */
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if (mid && mid > n / mid)
            high = mid - 1;
        else {
            ans = mid;
            low = mid + 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined offset, return 0 */

    /* position of the original Sturmian bit (k) and offset in its 3‑bit block (r) */
    idx_t k = (n - 1) / 3 + 1;    /* ceil(n/3) */
    idx_t r = (n - 1) % 3;        /* 0,1,2 */

    /* compute floor(k / sqrt(2)) and floor((k+1) / sqrt(2)) */
    idx_t kk  = k;
    idx_t fk  = isqrt_u64((idx_t)2 * kk * kk) / 2;

    idx_t kk1 = kk + (idx_t)1;
    idx_t fk1 = isqrt_u64((idx_t)2 * kk1 * kk1) / 2;

    /* Sturmian bit: 1 if floor((k+1)/sqrt(2)) > floor(k/sqrt(2)), 0 otherwise */
    seq_t sk = (fk1 > fk) ? (seq_t)1 : (seq_t)0;

    /* transform 0->101 , 1->010 : position r==1 gives sk, otherwise 1−sk */
    seq_t ans = (r == 1) ? sk : (seq_t)(1 - sk);
    return ans;
}

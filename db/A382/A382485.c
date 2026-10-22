/*
 * A382485  a(n) = ceiling(n/d^2) where d is the largest divisor of n which is
 *          not greater than the square root of n.
 * https://oeis.org/A382485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = ceil(n / d²), where d is the largest divisor of n not
 *            exceeding √n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer floor square root for 64‑bit indices (no floating point) */
static idx_t floor_sqrt(idx_t n)
{
    if (n < 2)                /* 0 or 1 */
        return n;
    idx_t low = 1;
    idx_t high = (n < 0xFFFFFFFFULL ? n : 0xFFFFFFFFULL);
    idx_t result = 1;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t mid2 = mid * mid;           /* mid ≤ 0xFFFFFFFF, so mid² fits in idx_t */
        if (mid2 == n)
            return mid;
        if (mid2 < n) {
            result = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; undefined for n==0 */
        return 0;

    /* largest divisor d ≤ √n */
    idx_t *sp = &n;      /* just to silence unused warning if any */
    (void)sp;            /* ignore */
    idx_t limit = floor_sqrt(n);
    idx_t d = 1;
    for (idx_t i = limit; i > 0; --i) {
        if (n % i == 0) {
            d = i;
            break;
        }
    }

    /* ceil(n / d²) without overflow */
    idx_t denom = d * d;
    idx_t q = n / denom;
    if (n % denom != 0)
        ++q;
    return (seq_t)q;
}

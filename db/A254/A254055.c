/*
 * A254055  Square array: A(row,col) = A003602(A254051(row,col)), read by
 *          antidiagonals A(1,1), A(1,2), A(2,1), A(1,3), A(2,2), A(3,1), ...
 * https://oeis.org/A254055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A003602(A254051(n)). For the cell (row,col) of
 *            antidiagonal n, A254051(row,col) = (1 + 3^(row-1)*v)/2 with v =
 *            2*floor(3*col/2) - 1, so a(n) = (A000265(Y) + 1)/2 with Y = 1 +
 *            3^(row-1)*v. First find 2^t dividing Y from Y mod 2^bits. Then
 *            compute Y/2^t exactly by multiplying the pair (q, r)...
 * Range:     u8 n <= 27, u16 n <= 75, u32 n <= 251, u64 n <= 945,
 *            u128 n <= 3566, big any n
 * Validated: standard, pass, 94 terms through n = 9760 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static unsigned A254055_width(void)
{
    seq_t p = 1;
    unsigned w = 0;
    while (p != 0 && w < 64) {
        p <<= 1;
        ++w;
    }
    return w;
}

seq_t a(idx_t n)
{
    idx_t d = 1, sum = 1;
    while (sum < n) {
        ++d;
        sum += d;
    }
    idx_t prev = sum - d;
    idx_t offset = n - prev;
    idx_t r = offset;
    idx_t c = d - offset + 1;

    seq_t v = 2 * ((seq_t)(3 * c) / 2) - 1;

    /* Y mod 2^bits (exact for arbitrary precision) to find its 2-adic valuation */
    seq_t y = v;
    for (idx_t i = 1; i < r; ++i) y *= 3;
    y += 1;

    unsigned cap = A254055_width() - 2;
    unsigned t = 0;
    if (y == 0) {
        t = cap;
    } else {
        seq_t z = y;
        while ((z & 1) == 0 && t < cap) {
            z >>= 1;
            ++t;
        }
    }
    if (t == 0) t = 1; /* Y is always even */

    seq_t mask = ((seq_t)1 << t) - 1;
    seq_t q = v >> t;
    seq_t rem = v & mask;
    for (idx_t i = 1; i < r; ++i) {
        seq_t s = 3 * rem;
        q = 3 * q + (s >> t);
        rem = s & mask;
    }
    /* P = q*2^t + rem, Y = P + 1 divisible by 2^t => rem + 1 == 2^t */
    seq_t o = q + ((rem + 1) >> t);
    while ((o & 1) == 0) o >>= 1;
    return (o + 1) >> 1;
}

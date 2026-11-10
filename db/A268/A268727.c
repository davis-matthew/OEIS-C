/*
 * A268727  One-based index of the toggled bit between n and A268717(n+1):
 *          a(n) = A070939(A003987(n,A268717(1+n))).
 * https://oeis.org/A268727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A001511(n)*(1-A010059(n)) + 1, where A010059(n) = 1 if n
 *            has an even number of 1-bits, else 0 (so a(0) = 1)
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 137 terms through n = 7615 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A268727_ruler(idx_t n) {
    seq_t r = 1;
    while ((n & 1) == 0) {
        r++;
        n >>= 1;
    }
    return r;
}

seq_t a(idx_t n) {
    idx_t t = n;
    unsigned par = 0;
    while (t) {
        par ^= (unsigned)(t & 1);
        t >>= 1;
    }
    if (n == 0 || par == 0) return (seq_t)1;
    return A268727_ruler(n) + 1;
}

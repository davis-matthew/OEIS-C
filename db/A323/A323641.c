/*
 * A323641  Triangle read by rows in which row n lists the first 2^n terms of
 *          A323642, n >= 1.
 * https://oeis.org/A323641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 125, u16 n <= 4093, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = (1 + (offset & 1)) * 3^popcount(offset >> 1)
 *  where offset = n - ((1 << floor_log2(n+1)) - 1)   (0‑based within its row) */
seq_t a(idx_t n)
{
    /* compute the row number r = floor(log2(n + 1)) */
    idx_t r = 0;
    idx_t limit = n + 1;
    while ((1ULL << (r + 1)) <= limit) {
        ++r;
    }

    /* starting 1‑based index of this row */
    idx_t row_start = (1ULL << r) - 1;

    /* zero‑based offset inside the row */
    idx_t offset = n - row_start;

    /* base value: 1 for even offset, 2 for odd offset */
    seq_t base = (seq_t)1 + (seq_t)(offset & 1);

    /* exponent of 3 is the popcount of offset >> 1 */
    idx_t bits = offset >> 1;
    idx_t e = 0;
    while (bits) {
        e += bits & 1;
        bits >>= 1;
    }

    /* compute 3^e */
    seq_t pow3 = (seq_t)1;
    for (idx_t i = 0; i < e; ++i) {
        pow3 *= (seq_t)3;
    }

    return base * pow3;
}

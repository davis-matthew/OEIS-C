/*
 * A284948  1-limiting word of the morphism 0 -> 10, 1 -> 00.
 * https://oeis.org/A284948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth letter of the 1‑limiting word of the morphism 0→10,
 *            1→00 (fixed point of the substitution 0→0010, 1→1010)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset 1 – a(1) = 1 */
    if (n == 0)
        return 0;       /* undefined, but harmless */

    idx_t idx = n - 1;          /* zero‑based index */
    idx_t digits[64];           /* base‑4 digits (max 32 for 64‑bit index) */
    int dcnt = 0;

    /* extract base‑4 digits (little‑endian) */
    while (idx > 0) {
        digits[dcnt++] = idx & 3;   /* low two bits */
        idx >>= 2;
    }

    unsigned char cur = 1;   /* start symbol of the fixed point is 1 */

    /* process digits from most significant to least */
    for (int i = dcnt - 1; i >= 0; --i) {
        unsigned char d = digits[i];
        if (cur == 0) {
            cur = (d == 2) ? 1 : 0;
        } else { /* cur == 1 */
            cur = (d == 0 || d == 2) ? 1 : 0;
        }
    }

    return (seq_t)cur;
}

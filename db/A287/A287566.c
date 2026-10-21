/*
 * A287566  Start with 0 and repeatedly substitute 0->0231, 1->2310, 2->3102,
 *          3->1023.
 * https://oeis.org/A287566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th digit of the fixed point of the morphism 0→0231,
 *            1→2310, 2→3102, 3→1023, starting with '0'. Indexing convention:
 *            a(1) is the first digit (1‑based)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned char _a287566_map[4][4] = {
    {0, 2, 3, 1},  /* 0 → 0 2 3 1 */
    {2, 3, 1, 0},  /* 1 → 2 3 1 0 */
    {3, 1, 0, 2},  /* 2 → 3 1 0 2 */
    {1, 0, 2, 3}   /* 3 → 1 0 2 3 */
};

seq_t a(idx_t n)
{
    if (n == 0)                /* offset 1; not used but harmless */
        return (seq_t)0;

    idx_t i = n - 1;           /* 0‑based index in the infinite word */
    unsigned char s = 0;       /* current symbol, starts from '0' */

    if (i == 0)
        return (seq_t)s;

    /* find the greatest power of 4 not exceeding i */
    idx_t pow = 1;
    while (pow <= i / 4)
        pow <<= 2;              /* multiply by 4 */

    /* process digits from most significant to least significant */
    while (pow > 0) {
        unsigned char child = (unsigned char)(i / pow);
        i %= pow;
        s = _a287566_map[s][child];
        pow >>= 2;              /* divide by 4 */
    }

    return (seq_t)s;
}

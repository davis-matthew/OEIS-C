/*
 * A287516  Start with 0 and repeatedly substitute 0->012, 1->102, 2->021.
 * https://oeis.org/A287516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th term of the fixed point of the morphism 0 → 012,
 *            1 → 102, 2 → 021. The value is obtained by expanding the
 *            morphism according to the base‑3 representation of n‑1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 1‑based index → 0‑based position for base‑3 encoding. */
    idx_t pos = n - 1;

    /* Transition table corresponding to the morphism. */
    const int trans[3][3] = {
        {0, 1, 2},   /* 0 → 012 */
        {1, 0, 2},   /* 1 → 102 */
        {0, 2, 1}    /* 2 → 021 */
    };

    /* Convert pos to base‑3 digits, least significant first. */
    idx_t tmp = pos;
    int digits[64];    /* enough for any 64‑bit index */
    int len = 0;
    do {
        digits[len++] = (int)(tmp % 3);
        tmp /= 3;
    } while (tmp > 0);

    /* Walk the morphism tree from the root (symbol 0),
       processing digits from most significant to least. */
    int cur = 0;
    for (int i = len - 1; i >= 0; --i)
        cur = trans[cur][digits[i]];

    return (seq_t)cur;
}

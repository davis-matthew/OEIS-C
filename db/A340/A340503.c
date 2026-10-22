/*
 * A340503  Fixed under 0 -> 02, 1 -> 32, 2 -> 01, 3 -> 31.
 * https://oeis.org/A340503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A340503: nth digit of the fixed point of the morphism
   0→02, 1→32, 2→01, 3→31, by traversing the binary expansion of (n‑1). */
seq_t a(idx_t n)
{
    seq_t digit = 0;
    idx_t x = n - 1;
    if (x == 0)
        return 0;                  /* a(1) = 0 */

    /* Find the position of the most significant bit of x. */
    int msb = -1;
    idx_t tmp = x;
    while (tmp)
    {
        ++msb;
        tmp >>= 1;
    }

    /* Mapping table: left child (bit 0) then right child (bit 1). */
    static const seq_t trans[4][2] = {
        {0, 2},     /* 0 → 0 2 */
        {3, 2},     /* 1 → 3 2 */
        {0, 1},     /* 2 → 0 1 */
        {3, 1}      /* 3 → 3 1 */
    };

    for (int i = msb; i >= 0; --i)
    {
        int bit = (x >> i) & 1;
        digit = trans[(int)digit][bit];
    }

    return digit;
}

/*
 * A283316  Image of 0 under repeated applications of the morphism 0 ->
 *          0,0,0,1, 1 -> 1,1,1,0.
 * https://oeis.org/A283316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A283316 –  Image of 0 under the morphism 0→0001, 1→1110
 *
 *  The nth term is obtained by writing n−1 in base‑4 and following that
 *  path through the substitution tree: starting from symbol 0, for each
 *  base‑4 digit r (taken from the most significant to the least)
 *      if the current symbol is 0 then the next symbol is 1 only when r==3,
 *            otherwise it stays 0;
 *      if the current symbol is 1 then the next symbol is 0 only when r==3,
 *            otherwise it stays 1.
 *
 *  This implements the infinite word obtained by iterating the uniform
 *  morphism 0→0001, 1→1110.
 */

seq_t a(idx_t n)
{
    /* convert n‑1 to base‑4 digits (least significant first) */
    uint64_t x = n - 1u;
    unsigned d[64];
    int len = 0;
    while (x > 0u) {
        d[len++] = (unsigned)(x & 3u);  /* x % 4 */
        x >>= 2;                        /* x /= 4 */
    }

    seq_t s = 0;   /* start from the initial symbol 0 */
    for (int i = len - 1; i >= 0; --i) {
        unsigned r = d[i];
        if (s == 0)
            s = (r == 3u) ? 1 : 0;
        else
            s = (r == 3u) ? 0 : 1;
    }
    return s;
}

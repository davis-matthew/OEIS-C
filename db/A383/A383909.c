/*
 * A383909  In the base 4 expansion of n, map: 0 -> 20, 1 -> 21, 2 -> 30, 3 ->
 *          31.
 * https://oeis.org/A383909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is obtained by writing n in base‑4, replacing each digit *
 *            0,1,2,3 by the two‑digit string 20,21,30,31 (in base‑4), and
 *            interpreting * the concatenation as a base‑4 number. The
 *            resulting value, expressed * in decimal, is the sequence term.
 *            Example: n = 4 (base‑4 10) → 20|10 * → 2120₂₀ → 152₁₀
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* special case n = 0 → base‑4 "0" → "20" → 8 */
    if (n == 0) return 8;

    static const seq_t mapv[4] = {8,9,12,13};  /* 20,21,30,31 as base‑4 values */

    idx_t t = n;
    unsigned char dig[32];  /* enough for 64‑bit input: log4(2^64)=32 digits */
    int len = 0;
    while (t > 0) {
        dig[len++] = (unsigned char)(t & 3);  /* least‑significant base‑4 digit */
        t >>= 2;                              /* divide by 4 */
    }

    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i)         /* from most‑significant digit */
        res = res * 16 + mapv[dig[i]];         /* append two base‑4 digits */

    return res;
}

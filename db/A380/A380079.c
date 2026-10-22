/*
 * A380079  Start with a list of the positive integers L in increasing order.
 *          Then, at turn n>=1, element n jumps from its current position, m,
 *          to position m+n. Then a(n) = L(m+1).
 * https://oeis.org/A380079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A380079
 *
 *  a(n) = n+1                     if  ⌊(t+1)·φ⌋ = n+1
 *  a(n) = t                       otherwise
 *
 *  where t = ⌊(n+1)/φ⌋ and φ = (1+√5)/2.
 *
 *  The values t and ⌊(t+1)·φ⌋ are obtained with integer arithmetic only.
 *  For an integer m ≥ 1:
 *      ⌊m/φ⌋   = ⌊(⌊√(5·m²)⌋ - m)/2⌋
 *      ⌊m·φ⌋   = ⌊(⌊√(5·m²)⌋ + m)/2⌋
 *
 *  The function is memoised; subsequent calls with larger n perform only
 *  a few 128‑bit integer multiplications and a single integer square‑root.
 */

#include <stdint.h>

/* Integer square‑root of a 128‑bit unsigned integer.
 * Returns ⌊√x⌋.  Uses a bit‑by‑bit algorithm that never overflows
 * by comparing via division. */
static inline unsigned __int128
isqrt128(unsigned __int128 x)
{
    unsigned __int128 r = 0;
    unsigned __int128 bit = (unsigned __int128)1 << 64;  /* 2^64 is > √(2^128) */

    while (bit)
    {
        unsigned __int128 nxt = r + bit;
        if (nxt <= x / nxt)   /* nxt² ≤ x  ⇔  nxt ≤ x / nxt  */
            r = nxt;
        bit >>= 1;
    }
    return r;
}

/*  a(n) for the OEIS sequence A380079  */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1 – undefined; return 0 for safety */
        return (seq_t)0;

    /* m = n + 1 */
    unsigned __int128 m = (unsigned __int128)(n + 1);

    /* S = ⌊√(5·m²)⌋ */
    unsigned __int128 S = isqrt128((unsigned __int128)5 * m * m);

    /* t = ⌊(S - m)/2⌋  = ⌊(n+1)/φ⌋  */
    unsigned __int128 t = (S - m) >> 1;      /* division by 2 */

    /* next = ⌊(t+1)·φ⌋  */
    unsigned __int128 tmp = t + 1;           /* t+1 */
    unsigned __int128 S2 = isqrt128((unsigned __int128)5 * tmp * tmp);
    unsigned __int128 next = (S2 + tmp) >> 1; /* division by 2 */

    /* a(n) = t if next == n+1, else n+1 */
    if (next == m)
        return (seq_t)t;
    else
        return (seq_t)m;
}

/*
 * A328544  Numbers k such that k multiplied by any of its digits contains k
 *          as a substring.
 * https://oeis.org/A328544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A328544: Numbers k such that k multiplied by any of its digits contains k as a substring.
   Sequence pattern: 0, 1, 5, 6, 11, 111, 1111, ...
   Formula: a(1)=0, a(2)=1, a(3)=5, a(4)=6,
            a(n)=repunit(n-3) for n≥5, where repunit(L)=10^(L-1)+...+10+1. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)6;

    idx_t len = n - 3;          /* length of the repunit */
    seq_t r = (seq_t)0;
    /* Build the repunit: multiply by 10 and add 1 for each decimal digit */
    while (len != 0) {
        r = r * (seq_t)10 + (seq_t)1;
        --len;
    }
    return r;
}

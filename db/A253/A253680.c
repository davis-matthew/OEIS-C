/*
 * A253680  Numbers c(n) whose square are equal to the sum of an odd number M
 *          of consecutive cubed integers b^3 + (b+1)^3 + ... + (b+M-1)^3 =
 *          c(n)^2, starting at b(n) (A253679).
 * https://oeis.org/A253680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * n * (n + 1) * (2 * n + 1) * (8 * n * (n + 1) + 1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 41, u64 n <= 3565,
 *            u128 n <= 9532, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* promote to 128‑bit arithmetic to avoid overflow in intermediate steps */
    unsigned __int128 x128 = (unsigned __int128)(seq_t)n;
    unsigned __int128 t   = x128 * (x128 + 1);            /* n*(n+1)          */
    unsigned __int128 p1  = (unsigned __int128)2 * x128 + 1;     /* 2n+1             */
    unsigned __int128 p2  = (unsigned __int128)8 * t + 1;        /* 8n(n+1)+1        */
    t = t * p1;  /* n(n+1)(2n+1)        */
    t = t * p2;  /* n(n+1)(2n+1)(8n(n+1)+1) */
    t = t * 2;   /* final *2             */
    return (seq_t)t;
}

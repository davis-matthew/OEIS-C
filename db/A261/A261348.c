/*
 * A261348  a(1)=0; a(2)=0; for n>2: a(n) = A237591(n,2) = A237593(n,2).
 * https://oeis.org/A261348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(1)=0, a(2)=0. For n ≥ 3: let r = (n − 3) mod 6. a(n) =
 *            ⌊(n−3)/6⌋ + 1 if r is 0, 1, 3 ⌊(n−3)/6⌋ + 2 otherwise
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    idx_t t = n - 3;                 /* n-3, guaranteed non‑negative */
    seq_t base = (seq_t)(t / 6) + 1; /* ⌊(n−3)/6⌋ + 1  */
    idx_t r   = t % 6;               /* (n−3) mod 6        */

    if (r == 0 || r == 1 || r == 3)
        return base;                 /* a(n) = base         */
    else
        return base + 1;             /* a(n) = base + 1     */
}

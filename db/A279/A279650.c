/*
 * A279650  An idempotent self-orthogonal Latin square of order 11, read by
 *          rows.
 * https://oeis.org/A279650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, tabf
 * Formula:   Let r = floor((n-1)/11) mod 11 (row index in {0,…,10}) Let c =
 *            (n-1) mod 11 (column index in {0,…,10}) Let s = (2*r) mod 11
 *            Then a(n) = (s + 1 - c) mod 11, with the convention that a value
 *            of 0 corresponds to 11
 * Range:     u8 n <= 121, u16 n <= 121, u32 n <= 121, u64 n <= 121,
 *            u128 n <= 121, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is at least 1 (offset is 1) */
    idx_t t = (n - 1) % 121;          /* reduce to one 11x11 block */
    idx_t r = t / 11;                 /* row within the block (0‑10) */
    idx_t c = t % 11;                 /* column within the block (0‑10) */
    idx_t s = (2 * r) % 11;           /* 2*r modulo 11 */
    idx_t valmod = (s + 1 + 11 - c) % 11;  /* ensures non‑negative before mod */
    return (valmod == 0) ? (seq_t)11 : (seq_t)valmod;
}

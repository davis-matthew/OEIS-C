/*
 * A306381  a(n) is the number of semimagic 4 X 3 matrices whose entries are
 *          nonnegative integers, with row sum = 3*n and column sum = 4*n.
 * https://oeis.org/A306381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (139/4)*n^6 + (417/4)*n^5 + (535/4)*n^4 + (375/4)*n^3 * +
 *            (77/2)*n^2 + 9*n + 1 (OEIS A306381)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 21, u64 n <= 899, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sequence value a(n).  Use a 128‑bit intermediate to avoid
 * overflow for all n that produce 64‑bit values, while remaining valid
 * for arbitrary‑precision builds. */
seq_t a(idx_t n)
{
    /* Horner's rule for the numerator 4*a(n): */
    /* 4*a(n) = (( ... ((139*n + 417)*n + 535)*n + 375)*n + 154)*n + 36)*n + 4 */
    unsigned __int128 num = 139;          /* 139 */
    unsigned __int128 nn  = n;            /* promote n for 128‑bit ops */

    num = num * nn + 417; /* ×n + 417 */
    num = num * nn + 535; /* ×n + 535 */
    num = num * nn + 375; /* ×n + 375 */
    num = num * nn + 154; /* ×n + 154 */
    num = num * nn + 36;  /* ×n + 36  */
    num = num * nn + 4;   /* ×n + 4   */

    num = num >> 2;       /* divide by 4 */

    return (seq_t)num;    /* cast to the configured output type */
}

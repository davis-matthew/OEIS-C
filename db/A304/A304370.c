/*
 * A304370  Number of function calls of the first kind required to compute
 *          ack(3,n), where ack denotes the Ackermann function.
 * https://oeis.org/A304370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 21*a(n-2) + 22*a(n-3) - 8*a(n-4) a(0)=9,
 *            a(1)=58, a(2)=283, a(3)=1244
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates A304370: number of first‑kind recursive calls in ack(3,n) */

seq_t a(idx_t n)
{
    if (n == 0) return 9;
    if (n == 1) return 58;
    if (n == 2) return 283;
    if (n == 3) return 1244;

    seq_t a0 = 9,  a1 = 58,  a2 = 283,  a3 = 1244,  next;
    idx_t i;

    for (i = 4; i <= n; ++i) {
        next = 8 * a3;   /* 8*a(n-1)   */
        next -= 21 * a2; /* -21*a(n-2) */
        next += 22 * a1; /* +22*a(n-3) */
        next -= 8 * a0;  /* -8*a(n-4)  */
        a0 = a1; a1 = a2; a2 = a3; a3 = next;
    }
    return a3;
}

/*
 * A269684  Number of length-n 0..2 arrays with no repeated value differing
 *          from the previous repeated value by plus or minus one modulo 2+1.
 * https://oeis.org/A269684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 48, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A269684
   a(n) = 4*a(n-1) - 3*a(n-2) - 2*a(n-3),
   a(1) = 3, a(2) = 9, a(3) = 27.  */

/* Computes a(n) for n >= 1.  Calls may arrive in any order. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is defined from offset 1; a(0) is unused. */
        return (seq_t)0;
    }
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)27;

    /* iterate from n = 4 up to the requested n.
       Keep only the last three terms to avoid large memory usage. */
    seq_t t0 = (seq_t)3;   /* a(n-3)  */
    seq_t t1 = (seq_t)9;   /* a(n-2)  */
    seq_t t2 = (seq_t)27;  /* a(n-1)  */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = t2 * (seq_t)4
                   - t1 * (seq_t)3
                   - t0 * (seq_t)2;
        t0 = t1;
        t1 = t2;
        t2 = next;
    }

    return t2;
}

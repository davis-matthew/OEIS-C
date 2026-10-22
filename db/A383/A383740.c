/*
 * A383740  a(0) = 4; a(n) = Pell(4*n)/Pell(n) for n > 0.
 * https://oeis.org/A383740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A383740
   Formula: a(0) = 4
            for n > 0: a(n) = Pell(4*n) / Pell(n)
   Recurrence:  a(n) = 12*a(n-1) + 30*a(n-2) - 12*a(n-3) - a(n-4),
                for n ≥ 4
   Initial values: a(0) = 4, a(1) = 12, a(2) = 204, a(3) = 2772
   This implementation uses the explicit recurrence, which
   works for any seq_t that supports the standard arithmetic
   operators. */

seq_t a(idx_t n)
{
    /* Handle the first four explicitly */
    if (n == (idx_t)0) return (seq_t)4;
    if (n == (idx_t)1) return (seq_t)12;
    if (n == (idx_t)2) return (seq_t)204;
    if (n == (idx_t)3) return (seq_t)2772;

    /* Recurrence uses the previous four terms */
    seq_t a0 = (seq_t)4;    /* a(n-4) */
    seq_t a1 = (seq_t)12;   /* a(n-3) */
    seq_t a2 = (seq_t)204;  /* a(n-2) */
    seq_t a3 = (seq_t)2772; /* a(n-1) */
    seq_t a4;              /* a(n) */

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        a4 = (seq_t)12 * a3 + (seq_t)30 * a2 - (seq_t)12 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3;
}

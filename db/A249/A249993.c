/*
 * A249993  Expansion of 1/((1+x)*(1+2*x)*(1-4*x)).
 * https://oeis.org/A249993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A249993
   Formula: a(n) = a(n-1) + 10*a(n-2) + 8*a(n-3),
   a(0)=1, a(1)=1, a(2)=11
   Recurrence works for all n >= 0.
   The function is valid for C11 and C++17 (seq_t may be an integer primitive
   or an arbitrary‑precision integer class with the usual operators). */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)11;

    /* iterate using the recurrence */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)11;  /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t a3 = a2 + (seq_t)10 * a1 + (seq_t)8 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}

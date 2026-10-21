/*
 * A253460  Indices of centered heptagonal numbers (A069099) which are also
 *          centered square numbers (A001844).
 * https://oeis.org/A253460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A253460
 * a(n) = a(n-1) + 254*a(n-2) - 254*a(n-3) - a(n-4) + a(n-5)
 * Initial values: a(1)=1, a(2)=16, a(3)=112, a(4)=3937, a(5)=28321
 */

seq_t a(idx_t n)
{
    /* return directly for the first five terms */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)16;
    if (n == 3) return (seq_t)112;
    if (n == 4) return (seq_t)3937;
    if (n == 5) return (seq_t)28321;

    /* rolling five‑value buffer for the recurrence */
    seq_t a1 = (seq_t)1;      /* a(i-5) */
    seq_t a2 = (seq_t)16;     /* a(i-4) */
    seq_t a3 = (seq_t)112;    /* a(i-3) */
    seq_t a4 = (seq_t)3937;   /* a(i-2) */
    seq_t a5 = (seq_t)28321;  /* a(i-1) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = a5 + (seq_t)254 * a4 - (seq_t)254 * a3 - a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }

    return a5;
}

/*
 * A369421  Expansion of o.g.f. x^3/((1-2*x)^3*(1-3*x)^2).
 * https://oeis.org/A369421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1)-57*a(n-2)+134*a(n-3)-156*a(n-4)+72*a(n-5),
 *            a(0)=a(1)=a(2)=0, a(3)=1, a(4)=12
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly */
    if (n == 0ull) return (seq_t)0;
    if (n == 1ull) return (seq_t)0;
    if (n == 2ull) return (seq_t)0;
    if (n == 3ull) return (seq_t)1;
    if (n == 4ull) return (seq_t)12;

    /* Last five terms: a(n-5), a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a0 = (seq_t)0;   /* a(n-5) */
    seq_t a1 = (seq_t)0;   /* a(n-4) */
    seq_t a2 = (seq_t)0;   /* a(n-3) */
    seq_t a3 = (seq_t)1;   /* a(n-2) */
    seq_t a4 = (seq_t)12;  /* a(n-1) */
    seq_t next;

    for (idx_t i = 5ull; i <= n; ++i)
    {
        /* Compute next term using the recurrence.
           The expression is split into positive and negative parts
           to avoid intermediate negative values when seq_t is unsigned. */
        seq_t pos = (seq_t)12 * a4 + (seq_t)134 * a2 + (seq_t)72 * a0;
        seq_t neg = (seq_t)57 * a3 + (seq_t)156 * a1;
        next = pos - neg;   /* final result is guaranteed to be non‑negative */

        /* Shift window: discard the oldest term and push the new one. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}

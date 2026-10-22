/*
 * A338532  Number of spanning trees in the n X 3 king graph.
 * https://oeis.org/A338532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=192, a(3)=17745, a(4)=1612127; For n>=5:
 *            a(n)=95*a(n-1)-384*a(n-2)+95*a(n-3)-a(n-4)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (The recurrence comes from the empirical generating function of
    the sequence.) */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)192;
    if (n == 3) return (seq_t)17745;
    if (n == 4) return (seq_t)1612127;

    seq_t v1 = (seq_t)1;          /* a(n-4) */
    seq_t v2 = (seq_t)192;        /* a(n-3) */
    seq_t v3 = (seq_t)17745;      /* a(n-2) */
    seq_t v4 = (seq_t)1612127;    /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t sum1 = (seq_t)95 * v4 + (seq_t)95 * v2;   /* 95*a(n-1)+95*a(n-3) */
        seq_t sum2 = (seq_t)384 * v3 + v1;              /* 384*a(n-2)+a(n-4) */
        seq_t next = sum1 - sum2;                       /* a(n) */
        v1 = v2; v2 = v3; v3 = v4; v4 = next;
    }
    return v4;
}

/*
 * A284840  Number of quinternary strings avoiding consecutive digits i,i+1
 *          and i,i+2.
 * https://oeis.org/A284840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 7*a(n-2) + 8*a(n-3) - 5*a(n-4) + a(n-5) a(0) =
 *            1, a(1) = 5, a(2) = 18, a(3) = 63, a(4) = 224
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)18;
    if (n == 3) return (seq_t)63;
    if (n == 4) return (seq_t)224;

    /* Store the last five values; a[0]=a(n-5), a[4]=a(n-1) */
    seq_t a[5];
    a[0] = (seq_t)1;   /* a(0) */
    a[1] = (seq_t)5;   /* a(1) */
    a[2] = (seq_t)18;  /* a(2) */
    a[3] = (seq_t)63;  /* a(3) */
    a[4] = (seq_t)224; /* a(4) */

    /* Compute a(n) for n >= 5 */
    for (idx_t k = 5; k <= n; ++k) {
        seq_t next = a[4] * (seq_t)5;   /* 5*a(n-1) */
        next -= a[3] * (seq_t)7;        /* -7*a(n-2) */
        next += a[2] * (seq_t)8;        /* +8*a(n-3) */
        next -= a[1] * (seq_t)5;        /* -5*a(n-4) */
        next += a[0];                  /* +a(n-5) */

        /* Shift the window: a[0]->a[1], ..., a[4]->next */
        a[0] = a[1];
        a[1] = a[2];
        a[2] = a[3];
        a[3] = a[4];
        a[4] = next;
    }

    return a[4];
}

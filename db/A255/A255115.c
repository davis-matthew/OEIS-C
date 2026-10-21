/*
 * A255115  Duplicate of A239333.
 * https://oeis.org/A255115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0)=1, a(1)=2, a(2)=5; a(n) = 2*a(n-1) + 2*a(n-3) for n >= 3
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 25, i64 n <= 50, i128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n)
    {
    case 0:
        return 1;
    case 1:
        return 2;
    case 2:
        return 5;
    }
    seq_t v0 = 1;      /* a(n-3) */
    seq_t v1 = 2;      /* a(n-2) */
    seq_t v2 = 5;      /* a(n-1) */
    seq_t next;
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t t1 = v2;    /* a(n-1) */
        t1 += t1;         /* 2*a(n-1) */
        seq_t t2 = v0;    /* a(n-3) */
        t2 += t2;         /* 2*a(n-3) */
        next = t1 + t2;   /* a(n) */
        v0 = v1;          /* shift window */
        v1 = v2;
        v2 = next;
    }
    return v2;
}

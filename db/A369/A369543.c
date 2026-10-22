/*
 * A369543  a(0) = 1; for n >= 0, a(n+1) = n - a(n) if a(n) odd, else a(n+1) =
 *          floor((3*n + a(n))/2).
 * https://oeis.org/A369543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1; for n >= 0: a(n+1) = n - a(n) if a(n) is odd,
 *            floor((3*n + a(n))/2) if a(n) is even
 * Range:     i8 n <= 25, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t prev = (seq_t)1;          /* a(0) */

    for (idx_t i = 0; i < n; ++i)
    {
        if (prev & 1)                /* prev is odd */
        {
            prev = (seq_t)i - prev;  /* a(i+1) = i - a(i) */
        }
        else                        /* prev is even */
        {
            seq_t num = (seq_t)i * 3 + prev;   /* 3*i + a(i) */
            seq_t next;
            if (num >= 0)            /* non‑negative, normal division */
                next = num / 2;
            else                     /* floor for negative values */
                next = -(((-num) + 1) / 2);
            prev = next;
        }
    }

    return prev;
}

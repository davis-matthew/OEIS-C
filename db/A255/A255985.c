/*
 * A255985  a(n) = 1 for n <= 6; a(n) = 49*a(n-1) - 882*a(n-2) + 8820*a(n-3) -
 *          52920*a(n-4) + 190512*a(n-5) - 381024*a(n-6) + 326592*a(n-7)
 *          otherwise.
 * https://oeis.org/A255985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 49*a(n-1) - 882*a(n-2) + 8820*a(n-3) - 52920*a(n-4) * +
 *            190512*a(n-5) - 381024*a(n-6) + 326592*a(n-7); * a(0) = a(1) =
 *            a(2) = a(3) = a(4) = a(5) = a(6) = 1 * * Compute using a sliding
 *            window of size 7. This works for all * integer types provided by
 *            the build (fixed‑size or arbitrary * prec...
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base case */
    if (n < 7) {
        return (seq_t)1;
    }

    /* Sliding window: t[0]..t[6] hold a(i-7)..a(i-1) */
    seq_t t[7];
    for (idx_t i = 0; i < 7; ++i) {
        t[i] = (seq_t)1;          /* a(0) .. a(6) = 1 */
    }

    for (idx_t i = 7; i <= n; ++i) {
        seq_t next =
            49 * t[6]   // a(i-1)
            - 882 * t[5]  // a(i-2)
            + 8820 * t[4] // a(i-3)
            - 52920 * t[3]// a(i-4)
            + 190512 * t[2]// a(i-5)
            - 381024 * t[1]// a(i-6)
            + 326592 * t[0]; // a(i-7)

        /* Shift the window left by one */
        for (idx_t j = 0; j < 6; ++j) {
            t[j] = t[j + 1];
        }
        t[6] = next;           /* t[6] now holds a(i) */
    }

    return t[6];
}

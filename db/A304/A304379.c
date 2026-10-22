/*
 * A304379  a(n) = 256n^2 - 828n + 656 (n>=1).
 * https://oeis.org/A304379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 256*n^2 - 828*n + 656
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The recurrence
       a(1) = 84
       a(n) = a(n-1) + 512*(n-1) - 572   for n > 1  */
    if (n == 1)
        return (seq_t)84;

    long long acc = 84;          /* use signed 64‑bit for intermediate */
    for (idx_t k = 1; k < n; ++k)
    {
        long long diff = 512LL * (long long)k - 572LL;
        acc += diff;
    }
    return (seq_t)acc;
}

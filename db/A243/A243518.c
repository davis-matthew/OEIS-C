/*
 * A243518  Number of length n+2 0..9 arrays with no three elements in a row
 *          with pattern aba or abb (with a!=b) and new values 0..9 introduced
 *          in 0..9 order.
 * https://oeis.org/A243518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*a(n-1) - 372*a(n-2) + 2478*a(n-3) - 9639*a(n-4) * +
 *            22260*a(n-5) - 29588*a(n-6) + 20592*a(n-7) - 5760*a(n-8); *
 *            Initial values: * a(1)=3, a(2)=5, a(3)=10, a(4)=25, a(5)=77, *
 *            a(6)=280, a(7)=1157, a(8)=5297
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 26, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first eight terms directly. */
    if (n == 1u) return (seq_t)3;
    if (n == 2u) return (seq_t)5;
    if (n == 3u) return (seq_t)10;
    if (n == 4u) return (seq_t)25;
    if (n == 5u) return (seq_t)77;
    if (n == 6u) return (seq_t)280;
    if (n == 7u) return (seq_t)1157;
    if (n == 8u) return (seq_t)5297;

    /* Window of the last eight terms:  A[i-8] ... A[i-1]  */
    seq_t w[8] = {
        (seq_t)3,   /* A[1]  */
        (seq_t)5,   /* A[2]  */
        (seq_t)10,  /* A[3]  */
        (seq_t)25,  /* A[4]  */
        (seq_t)77,  /* A[5]  */
        (seq_t)280, /* A[6]  */
        (seq_t)1157,/* A[7]  */
        (seq_t)5297 /* A[8]  */
    };

    seq_t next;
    for (idx_t i = 9u; i <= n; ++i)
    {
        /* Compute A[i] using the recurrence. */
        next =
            w[7] * (seq_t)30   - w[6] * (seq_t)372  + w[5] * (seq_t)2478
          - w[4] * (seq_t)9639  + w[3] * (seq_t)22260 - w[2] * (seq_t)29588
          + w[1] * (seq_t)20592 - w[0] * (seq_t)5760;

        /* Shift the window: drop the oldest term and add the new one. */
        for (idx_t j = 0; j < 7; ++j)
            w[j] = w[j + 1];
        w[7] = next;
    }

    return w[7];
}

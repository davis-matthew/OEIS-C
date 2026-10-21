/*
 * A264111  Number of (1+1) X (n+1) arrays of permutations of 0..n*2+1 with
 *          each element having index change +-(.,.) 0,0 0,2 or 1,1.
 * https://oeis.org/A264111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) + 15*a(n-4) - 60*a(n-5) + 15*a(n-6) *
 *            -15*a(n-8) + 60*a(n-9) -15*a(n-10) + a(n-12) - 4*a(n-13) +
 *            a(n-14) * Recurrence order: 14, initial values a(1) … a(14)
 *            given below
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for the undefined offset 0, for safety. */
    if (n == (idx_t)0) {
        return (seq_t)0;
    }

    /* Initial values (a(1) … a(14)) as given by OEIS. */
    static const seq_t base[15] = {
        0ULL,                      /* base[0] unused */
        2ULL, 9ULL, 45ULL, 144ULL, 512ULL, 1936ULL,
        7381ULL, 27225ULL, 101250ULL, 378225ULL,
        1413721ULL, 5271616ULL, 19668992ULL, 73410624ULL
    };

    if (n < 15) {
        return base[(idx_t)n];
    }

    /* Circular buffer of size 15 – indices 0 … 14 correspond to a(n-14) … a(n-1) */
    seq_t buf[15];
    for (idx_t i = 1; i < 15; ++i) {
        buf[i] = base[i];
    }
    buf[0] = base[14];  /* a(14) goes into position 0 for future modulo arithmetic */

    for (idx_t i = 15; i <= n; ++i) {
        /* Compute a(i) from the recurrence using modular indexing. */
        seq_t val =
            (seq_t)4  * buf[(i - 1) % 15] - buf[(i - 2) % 15]
          + (seq_t)15 * buf[(i - 4) % 15] - (seq_t)60 * buf[(i - 5) % 15]
          + (seq_t)15 * buf[(i - 6) % 15] - (seq_t)15 * buf[(i - 8) % 15]
          + (seq_t)60 * buf[(i - 9) % 15] - (seq_t)15 * buf[(i -10) % 15]
          + buf[(i -12) % 15] - (seq_t)4 * buf[(i -13) % 15]
          + buf[(i -14) % 15];

        buf[i % 15] = val;
    }

    return buf[n % 15];
}

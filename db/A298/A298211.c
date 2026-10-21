/*
 * A298211  Smallest n such that A001353(a(n)) == 0 (mod n), i.e.,
 *          x=A001075(a(n)) and y=A001353(a(n)) is the fundamental solution of
 *          the Pell equation x^2 - 3*(n*y)^2 = 1.
 * https://oeis.org/A298211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 1 such that A001353(k) ≡ 0 (mod n)
 * Range:     u8 n <= 484, u16 n <= 9893, u32 n <= 9893, u64 n <= 9893,
 *            u128 n <= 9893, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes a(n) by iterating the Pell recurrence for x² – 3y² = 1
   modulo n, stopping when y ≡ 0 (mod n). The guaranteed bound a(n) ≤ n
   ensures the loop terminates within at most n steps. */

seq_t a(idx_t n)
{
    if (n == 0)   /* not reached for the OEIS offset, but defensive */
        return 0;

    /* modulus used in the recurrence – n fits into seq_t by construction */
    seq_t mod = (seq_t)n;

    /* initial (x, y) = (2, 1) for the fundamental solution of x² – 3y² = 1 */
    seq_t x = 2 % mod;
    seq_t y = 1 % mod;

    for (idx_t k = 1; k <= n; ++k) {
        if (y == 0)
            return (seq_t)k;   /* a(n) found */

        /* Update using the Pell recurrence:
           x' = 2x + 3y
           y' =  x + 2y   */
        unsigned int x_u = (unsigned int)x;
        unsigned int y_u = (unsigned int)y;
        unsigned int mod_u = (unsigned int)mod;

        unsigned int x_next = (2 * x_u + 3 * y_u) % mod_u;
        unsigned int y_next = (x_u + 2 * y_u) % mod_u;

        x = (seq_t)x_next;
        y = (seq_t)y_next;
    }

    /* Should never reach here – a(n) always ≤ n */
    return (seq_t)n;
}

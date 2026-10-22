/*
 * A322983  Number of iterations of A011371(x) = x - A000120(x) needed to
 *          reach an odd number, when starting from x = n.
 * https://oeis.org/A322983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 0 if n is odd; a(n) = 1 + a(n - popcount(n)) if n is even
 * Range:     u8 n <= 9883, u16 n <= 9883, u32 n <= 9883, u64 n <= 9883,
 *            u128 n <= 9883, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A011371(x) = x - A000120(x) computes x minus the number of 1 bits in x. */

/* Computes the number of iterations needed to reach an odd number
   starting from n, following the recurrence
   a(n) = 0            if n is odd
   a(n) = 1 + a(n - popcount(n))   if n is even. */
seq_t a(idx_t n) {
    /* An odd starting value reaches odd immediately. */
    if ((n & 1) != 0)
        return (seq_t)0;

    idx_t x = n;
    seq_t steps = 0;

    /* Iterate until x becomes odd. */
    while ((x & 1) == 0) {
        unsigned int pc = __builtin_popcountll(x);  /* population count of x */
        x -= pc;
        steps += (seq_t)1;
    }
    return steps;
}

/*
 * A278586  Start with X = n^2. Repeatedly replace X with X - ceiling(X/n);
 *          a(n) is the number of steps to reach 0.
 * https://oeis.org/A278586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 323, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A278586: iterate X <- X - ceil(X/n) starting from X = n^2 until X = 0;
    a(n) is the number of iterations (steps) needed. */
seq_t a(idx_t n)
{
    /* avoid division by zero for n==0 (out‑of‑range call) */
    if (n == 0)
        return 0;

    seq_t X   = (seq_t)n * (seq_t)n;   /* start value n² */
    seq_t n_s = (seq_t)n;              /* cast n for arithmetic */
    seq_t steps = 0;

    while (X > 0) {
        /* ceil(X / n) = (X + n - 1) / n */
        seq_t sub = (X + n_s - 1) / n_s;
        X -= sub;
        ++steps;
    }
    return steps;
}

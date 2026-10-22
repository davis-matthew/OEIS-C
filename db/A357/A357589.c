/*
 * A357589  a(n) = n - A130312(n).
 * https://oeis.org/A357589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - A130312(n)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* A130312(n) is the Fibonacci number that precedes n in the Fibonacci
       sequence.  The function finds the smallest Fibonacci number greater
       than n, then subtracts the preceding Fibonacci number from n. */
    seq_t target = (seq_t)n;
    if (n == 0)      /* protects against out‑of‑range calls */
        return 0;

    /* Generate Fibonacci numbers until the next one exceeds n. */
    seq_t f0 = 1,           /* F_k  (current)       */
         f1 = 1,           /* F_{k+1} (next)       */
         f_prevprev = 1;   /* F_{k-1} (previous)   */

    while (f1 <= target) {
        seq_t f_next = f0 + f1;
        f_prevprev = f0;    /* shift one step: new F_{k-1} */
        f0 = f1;            /* advance to next Fibonacci   */
        f1 = f_next;
    }

    /* f_prevprev now holds A130312(n). */
    seq_t result = target - f_prevprev;
    return result;
}

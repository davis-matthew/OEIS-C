/*
 * A256967  a(n) = A256966(n) + 1.
 * https://oeis.org/A256967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A256966(n) + 1, where A256966 increments 1, 2, 3, … each
 *            repeated F_k times (F_k Fibonacci with F_1 = F_2 = 1)
 * Range:     u8 n <= 42, u16 n <= 4248, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    idx_t r = n - 1;          /* number of increments after the first term */
    seq_t R = (seq_t)r;       /* promote to seq_t for arithmetic */

    seq_t F_prev = 0;         /* F_0 */
    seq_t F_curr = 1;         /* F_1 */
    idx_t k = 1;              /* current k (diff value) */

    seq_t cum  = 0;           /* Σ F_i for i < k */
    seq_t wsum = 0;           /* Σ i * F_i for i < k */

    while (cum + F_curr <= R) {
        wsum += (seq_t)k * F_curr;
        cum  += F_curr;

        ++k;
        seq_t next = F_curr + F_prev;
        F_prev = F_curr;
        F_curr = next;
    }

    seq_t remaining = R - cum;                 /* increments in the last partial group */
    seq_t diff      = (seq_t)k;                /* diff value for that group */

    seq_t total = wsum + diff * remaining;     /* sum of all increments */
    return (seq_t)1 + total;                   /* a(1) = 1 */
}

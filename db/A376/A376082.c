/*
 * A376082  a(n) is the period of the Leonardo numbers (A001595) modulo n.
 * https://oeis.org/A376082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pisano_period(n) if n is odd, * a(n) = pisano_period(n/2)
 *            if n is even. * The period of Fibonacci numbers modulo m is
 *            computed * by iterating the recurrence F_k+1 = F_k + F_k-1 (mod
 *            m) * until the pair (0,1) reappears
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t mod = (n & 1) ? n : n >> 1;   /* m = n/(n%2?1:2)       */
    if (mod == 1)                       /* Pisano period of 1 is 1 */
        return (seq_t)1;

    idx_t prev   = 0;
    idx_t curr   = 1 % mod;
    idx_t period = 0;

    /* Find the smallest P such that (F_P mod mod, F_{P+1} mod mod) = (0,1) */
    do
    {
        ++period;
        idx_t next = (prev + curr) % mod;
        prev = curr;
        curr = next;
    } while (!(prev == 0 && curr == 1));

    return (seq_t)period;
}

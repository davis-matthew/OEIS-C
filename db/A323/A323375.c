/*
 * A323375  Let f(p, q) denote the pair (p + q, wt(p) + wt(q)). a(n) gives the
 *          number of iterations of f starting at (n, 1) needed to make p/q an
 *          integer, or if no integer is ever reached then a(n) = -1. (Here wt
 *          is binary weight, A000120.)
 * https://oeis.org/A323375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of iterations of f starting at (n,1) until p/q is
 *            an integer. Initial values (computed by the function): a(1)=1,
 *            a(2)=3, a(3)=3, a(4)=3, a(5)=1, ... (see OEIS entry)
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes binary weight of an unsigned integer of type seq_t. */
static inline seq_t popcount_seq(seq_t x)
{
    seq_t cnt = 0;
    while (x != 0)
    {
        cnt += x & 1;
        x >>= 1;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* start at (p, q) = (n, 1) */
    seq_t p = n;
    seq_t q = 1;
    idx_t count = 0;

    /* maximum iterations to detect non‑terminating behaviour */
    const idx_t MAX_STEPS = 2000000;

    for (;;)
    {
        /* apply f : (p, q) -> (p+q, wt(p)+wt(q)) */
        seq_t p_old = p;
        seq_t q_old = q;
        p = p + q;
        seq_t wp = popcount_seq(p_old);
        seq_t wq = popcount_seq(q_old);
        q = wp + wq;
        count++;

        if ((p % q) == 0)
            return (seq_t)count;  /* reached an integer ratio */

        if (count >= MAX_STEPS)
            return (seq_t)-1;     /* assume never reaching an integer */
    }
}

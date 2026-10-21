/*
 * A286326  Least possible maximum of the two initial terms of a
 *          Fibonacci-like sequence containing n.
 * https://oeis.org/A286326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A286326: the least possible maximum of the two initial
 * terms of a Fibonacci‑like sequence containing n.
 * Formula: minimising max(a,b) subject to a*F_{k-1}+b*F_k=n, a,b≥0.
 *
 * The implementation pre‑computes a fixed list of Fibonacci numbers
 * (int64‑wide) and then, for each k with F_{k-1}≤n, searches the
 * unique solution a∈[0,n/F_{k-1}] presenting a valid b.  The smallest
 * resulting max(a,b) is returned.
 */

seq_t a(idx_t n)
{
    /* Special case: n = 0 */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Lowest possible maximum is n (take a = n, b = 0). */
    idx_t best = n;

    /* Pre‑compute Fibonacci numbers up to a fixed limit. */
    static idx_t fib[200];
    static int fib_init = 0;

    if (!fib_init) {
        fib[0] = (idx_t)0;
        fib[1] = (idx_t)1;
        for (idx_t i = 2; i < (idx_t)200; ++i)
            fib[i] = fib[i - 1] + fib[i - 2];
        fib_init = 1;
    }

    /* For each k>=2 with F_{k-1}≤n, try to minimise max(a,b). */
    for (idx_t k = 2; k < (idx_t)200; ++k) {
        if (fib[k - 1] > n)
            break;                /* further k will give F_{k-1}>n */

        idx_t a_max = n / fib[k - 1];   /* a*F_{k-1} ≤ n */
        for (idx_t a = (idx_t)0; a <= a_max; ++a) {
            idx_t rem = n - a * fib[k - 1];
            if (rem % fib[k] != (idx_t)0)
                continue;           /* not divisible, no integer b */

            idx_t b = rem / fib[k];
            /* b is non‑negative by construction of a_max. */

            idx_t cand = a > b ? a : b;
            if (cand < best)
                best = cand;
        }
    }

    return (seq_t)best;
}

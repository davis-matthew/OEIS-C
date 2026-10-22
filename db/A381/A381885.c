/*
 * A381885  a(n) = Product_{k=2..n-1} k^ord(n, k) where ord(n, k) = 0 if k
 *          does not divide n, otherwise is the exponent of the highest power
 *          of k that divides n.
 * https://oeis.org/A381885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=2..n-1} k^ord(n, k) where ord(n,k) is the
 *            exponent of the largest power of k dividing n
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 47, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 */

    /* ----- factor n into primes ----- */
    idx_t m = n;
    idx_t primes[64];
    int    exps [64];
    int    pf  = 0;

    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            primes[pf] = p;
            exps [pf] = e;
            ++pf;
        }
    }
    if (m > 1)          /* remaining prime factor */
    {
        primes[pf] = m;
        exps [pf] = 1;
        ++pf;
    }

    /* ----- generate all divisors of n (including 1) ----- */
    idx_t divs[1024];
    int    divc = 1;
    divs[0] = 1;

    for (int i = 0; i < pf; ++i)
    {
        idx_t p = primes[i];
        int   e = exps[i];
        int   current = divc;
        for (int j = 0; j < current; ++j)
        {
            idx_t val = divs[j];
            for (int k = 1; k <= e; ++k)
            {
                val *= p;
                divs[divc++] = val;
            }
        }
    }

    /* ----- compute the product ----- */
    seq_t result = (seq_t)1;

    for (int i = 0; i < divc; ++i)
    {
        idx_t d = divs[i];
        if (d < 2) continue;              /* skip 1           */

        /* ord(n, d) : highest e such that d^e | n */
        idx_t e = 0;
        idx_t t = n;
        while (t % d == 0)
        {
            t /= d;
            ++e;
        }
        if (e == 0) continue;

        /* multiply by d^e */
        seq_t pow = (seq_t)1;
        for (int k = 0; k < e; ++k) pow *= (seq_t)d;
        result *= pow;
    }

    /*  divide by n to remove the factor from k = n   */
    result /= (seq_t)n;
    return result;
}

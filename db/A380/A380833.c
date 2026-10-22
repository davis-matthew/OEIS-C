/*
 * A380833  a(n) is the number of divisors d of n satisfying (-d)^n mod n = d.
 * https://oeis.org/A380833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that (-d)^n ≡ d (mod n)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute d^exp mod mod, where exp is an idx_t (uint64_t). */
static seq_t mod_pow(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    seq_t b = base % mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t cnt = 0;
    seq_t nseq = (seq_t)n;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            seq_t d1 = (seq_t)i;
            seq_t pow1 = mod_pow(d1, n, nseq);
            if (n & 1ULL)            /* odd exponent: apply minus sign */
                pow1 = (nseq - pow1) % nseq;
            if (pow1 == d1)
                ++cnt;

            if (i != n / i)          /* second divisor n/i */
            {
                seq_t d2 = (seq_t)(n / i);
                seq_t pow2 = mod_pow(d2, n, nseq);
                if (n & 1ULL)
                    pow2 = (nseq - pow2) % nseq;
                if (pow2 == d2)
                    ++cnt;
            }
        }
    }
    return cnt;
}

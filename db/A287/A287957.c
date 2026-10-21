/*
 * A287957  Table read by antidiagonals: T(n, k) = greatest common recursive
 *          divisor of n and k; n > 0 and k > 0.
 * https://oeis.org/A287957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, mult
 * Formula:   a(n) = recursive GCD of the coordinates (i,j) that * correspond
 *            to the n‑th element of the table read by antidiagonals. * The
 *            recursive GCD is defined on the prime tower factorizations * of
 *            the two numbers: for every common prime p the exponent of p * in
 *            the result is the recursive GCD of the cor...
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* -------------- auxiliary: triangular number --------------- */
static inline idx_t triangular(idx_t d)
{
    /* compute d*(d+1)/2 without overflow using 128‑bit arithmetic */
    return (idx_t)(((__int128)d * (d + 1)) / 2);
}

/* -------------- auxiliary: prime factorisation for small ints --------------- */
static size_t factor_small(uint64_t n, uint64_t primes[10], unsigned exps[10])
{
    size_t cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            unsigned e = 0;
            while (n % p == 0)
            {
                n /= p;
                e++;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            cnt++;
        }
    }
    if (n > 1)
    {
        primes[cnt] = n;
        exps[cnt]   = 1;
        cnt++;
    }
    return cnt;
}

/* -------------- recursive GCD of two numbers ------------------- */
static seq_t recursive_factor_gcd(seq_t a, seq_t b)
{
    /* base case: if either number is 1, the result is 1 */
    if (a == 1 || b == 1)
        return (seq_t)1;

    /* work with 64‑bit integers (they are small enough in this sequence) */
    uint64_t ua = (uint64_t)a;
    uint64_t ub = (uint64_t)b;

    /* factor both numbers */
    uint64_t pa[10], pb[10];
    unsigned ea[10], eb[10];
    size_t ca = factor_small(ua, pa, ea);
    size_t cb = factor_small(ub, pb, eb);

    seq_t res = (seq_t)1;

    /* for each common prime compute the recursive exponent */
    for (size_t i = 0; i < ca; ++i)
    {
        for (size_t j = 0; j < cb; ++j)
        {
            if (pa[i] == pb[j])
            {
                /* exponent is the recursive GCD of the exponents */
                seq_t e_common = recursive_factor_gcd((seq_t)ea[i], (seq_t)eb[j]);
                seq_t term     = seq_pow((seq_t)pa[i], (idx_t)e_common);
                res = seq_mul(res, term);
                break;
            }
        }
    }
    return res;
}

/* -------------- main sequence function ---------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* offset starts at 1 – 0 is undefined */

    /* find the diagonal d such that triangular(d) >= n */
    idx_t low = 1;
    idx_t high = 2 * n;          /* upper bound is always sufficient */
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) < n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t d = low;

    idx_t prev = triangular(d - 1);
    idx_t i    = n - prev;          /* row number (1‑based) */
    idx_t j    = d - i + 1;         /* column number (1‑based) */

    return recursive_factor_gcd((seq_t)i, (seq_t)j);
}

/*
 * A318637  Expansion of Sum_{n>=1} ( (2 + x^n)^n - 2^n ).
 * https://oeis.org/A318637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 2^(d - n/d) * binomial(d, n/d)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* ---------- Small trial‑division factorisation ----------
   Fills arrays pr[0..k-1] and ex[0..k-1] with the
   prime bases and their exponents, and stores k in *cnt.
   Works for any n that fits in idx_t. -----------------*/
static void factor_simple(idx_t n, idx_t *pr, idx_t *ex, size_t *cnt)
{
    size_t k = 0;
    idx_t t = n;
    for (idx_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            size_t e = 0;
            while (t % p == 0)
            {
                t /= p;
                ++e;
            }
            pr[k] = p;
            ex[k] = (idx_t)e;
            ++k;
        }
    }
    if (t > 1)
    {
        pr[k] = t;
        ex[k] = (idx_t)1;
        ++k;
    }
    *cnt = k;
}

/* ---------- Recursively generate all divisors ----------
   divs[0]..divs[*dcount-1] will contain all divisors of n.
   The recursion depth is the number of distinct prime factors. --*/
static void gen_divs_rec(const idx_t *pr, const idx_t *ex,
                         size_t idx, size_t cnt, idx_t cur,
                         idx_t *divs, size_t *dcount)
{
    if (idx == cnt)
    {
        divs[*dcount] = cur;
        ++(*dcount);
        return;
    }
    idx_t val = 1;
    for (idx_t e = 0; e <= ex[idx]; ++e)
    {
        gen_divs_rec(pr, ex, idx + 1, cnt, cur * val, divs, dcount);
        val *= pr[idx];
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* 1. Factor n */
    idx_t primes[64];
    idx_t exps[64];
    size_t prime_cnt = 0;
    factor_simple(n, primes, exps, &prime_cnt);

    /* 2. Enumerate all divisors of n */
    idx_t divs[1024];
    size_t div_cnt = 0;
    gen_divs_rec(primes, exps, 0, prime_cnt, (idx_t)1, divs, &div_cnt);

    /* 3. Accumulate the divisor‑sum formula */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < div_cnt; ++i)
    {
        idx_t d = divs[i];
        idx_t e = n / d;          /* d | n */
        if (d < e)                /* binomial will be 0 here */
            continue;

        /* exponent of 2 */
        idx_t exp = d - e;
        seq_t term = binomial(d, e) * pow2(exp);
        sum += term;
    }

    return sum;
}

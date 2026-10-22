/*
 * A378008  a(n) = b(5*n+1), with the sequence {b(n)} having Dirichlet g.f.
 *          Product_{chi} L(chi,s), where chi runs through all Dirichlet
 *          characters modulo 5; 5th column of A378007.
 * https://oeis.org/A378008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(5*n+1) where b is multiplicative: - b(5^e) = 0 - if p ≡
 *            1 (mod 5): b(p^e) = C(e+3,3) - if p ≡ 4 (mod 5): b(p^e) = e/2+1
 *            (e even), 0 (e odd) - if p ≡ 2 or 3 (mod 5): b(p^e) = 1 (e
 *            divisible by 4), 0 otherwise The function is fully deterministic
 *            and memoised via a small SPF sieve
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* dynamic SPF (smallest prime factor) table */
static uint32_t *spfs = NULL;
static idx_t spf_limit = 0;  /* maximum index spfs covers */

static void build_spf(idx_t limit)
{
    if (spf_limit >= limit)
        return;                      /* already built enough */
    free(spfs);
    spfs = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    if (!spfs)
        return;                      /* out of memory – the caller should not hit this */
    for (idx_t i = 2; i <= limit; ++i)
        spfs[i] = (uint32_t)i;
    idx_t root = (idx_t)(limit < (idx_t)46341 ? limit : 46341); /* sqrt(2^32) */
    for (idx_t p = 2; p <= root; ++p)
    {
        if (spfs[p] != (uint32_t)p)   /* not prime */
            continue;
        for (idx_t j = p * p; j <= limit; j += p)
            if (spfs[j] == (uint32_t)j)
                spfs[j] = (uint32_t)p;
    }
    spf_limit = limit;
}

/* multiplicative function b(n) as described */
static seq_t b_value(idx_t n)
{
    if (n == 1)          /* empty product */
        return (seq_t)1;

    build_spf(n);        /* ensure spf table covers n */
    seq_t result = (seq_t)1;
    while (n > 1)
    {
        uint32_t p = spfs[n];
        if (p == 5)      /* any 5 factor kills the product */
            return (seq_t)0;
        idx_t exp = 0;
        while (n % p == 0)
        {
            n /= p;
            ++exp;
        }
        seq_t val = (seq_t)1;
        if (p % 5 == 1)                /* binomial(e+3,3) */
        {
            seq_t e = (seq_t)exp;
            val = (e + 3) * (e + 2) * (e + 1) / 6;
        }
        else if (p % 5 == 4)          /* e even? */
        {
            if (exp % 2 == 0)
                val = (seq_t)(exp / 2 + 1);
            else
                return (seq_t)0;
        }
        else                            /* p % 5 == 2 or 3 */
        {
            if (exp % 4 == 0)
                val = (seq_t)1;
            else
                return (seq_t)0;
        }
        result *= val;
    }
    return result;
}

extern seq_t a(idx_t n)
{
    /* a(n) = b(5*n+1) */
    idx_t m = 5 * n + 1;
    return b_value(m);
}

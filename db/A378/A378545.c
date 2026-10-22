/*
 * A378545  a(n) is the sum of those divisors d of n for which A083345(d) is
 *          odd, where A083345(n) is the numerator of Sum(e/p:
 *          n=Product(p^e)).
 * https://oeis.org/A378545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, A083345(d) odd} d
 * Range:     u8 n <= 119, u16 n <= 9915, u32 n <= 9915, u64 n <= 9915,
 *            u128 n <= 9915, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The condition “A083345(d) odd” is equivalently: */
/*   Let n = ∏ p_i^{e_i}.  Define M = ∏ p_i  (product of distinct primes). */
/*   Compute S = Σ e_i * (M / p_i).  Then: */
/*     - If d is odd, A083345(d) is odd ⇔ S is odd. */
/*     - If d is even, A083345(d) is odd ⇔ S mod 4 ≠ 0.   */
#include <stdint.h>
#include <stddef.h>

/* Very small prime factoriser for uint64_t values.  n should fit in idx_t. */
static void factorize(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    if (n < 2)
    {
        *cnt = 0;
        return;
    }
    if ((n & 1) == 0)
    {
        uint64_t e = 0;
        while ((n & 1) == 0)
        {
            n >>= 1;
            ++e;
        }
        pr[c] = 2;
        ex[c] = e;
        ++c;
    }
    for (uint64_t p = 3; p * p <= n; p += 2)
    {
        if (n % p == 0)
        {
            uint64_t e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            pr[c] = p;
            ex[c] = e;
            ++c;
        }
    }
    if (n > 1)
    {
        pr[c] = n;
        ex[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Recursive enumeration of divisors. */
static void rec(const uint64_t *pr, const uint64_t *ex, size_t m,
                size_t pos, seq_t current_d,
                uint64_t *cur_exp, seq_t *sum)
{
    if (pos == m)
    {
        /* Build M = product of primes with exponent > 0. */
        seq_t M = 1;
        for (size_t i = 0; i < m; ++i)
        {
            if (cur_exp[i] > 0)
                M *= (seq_t)pr[i];
        }

        /* Compute S = Σ exp_i * (M / p_i). */
        seq_t S = 0;
        for (size_t i = 0; i < m; ++i)
        {
            if (cur_exp[i] > 0)
                S += (seq_t)cur_exp[i] * (M / (seq_t)pr[i]);
        }

        int qualifies;
        if ((current_d & 1) != 0)         /* d odd */
            qualifies = (S & 1) != 0;     /* S odd */
        else                             /* d even */
            qualifies = ((S & 3) != 0);   /* S % 4 != 0 */

        if (qualifies)
            *sum += current_d;
        return;
    }

    seq_t d = current_d;
    for (uint64_t e = 0; e <= ex[pos]; ++e)
    {
        cur_exp[pos] = e;
        rec(pr, ex, m, pos + 1, d, cur_exp, sum);
        if (e < ex[pos])
            d *= (seq_t)pr[pos];
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Factorise n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t m;
    factorize((uint64_t)n, primes, exps, &m);

    /* Prepare arrays for recursion. */
    uint64_t cur_exp[64] = {0};
    seq_t result = (seq_t)0;

    rec(primes, exps, m, 0, (seq_t)1, cur_exp, &result);
    return result;
}

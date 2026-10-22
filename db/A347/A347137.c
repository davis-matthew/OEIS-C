/*
 * A347137  a(n) = Sum_{d|n} phi(d) * A003961(n/d), where A003961 shifts the
 *          prime factorization of its argument one step towards larger
 *          primes, and phi is Euler totient function.
 * https://oeis.org/A347137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} phi(d) * A003961(n/d) * (Dirichlet convolution
 *            of Euler's phi with prime-shift function)
 * Range:     u8 n <= 23, u16 n <= 719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, next_prime, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Utilities provided by the framework */
#include "../utils/factor_u64.h"   /* factor_u64 */
#include "../utils/next_prime.h"   /* next_prime */
#include "../utils/phi.h"          /* seq_t phi(idx_t) */

/* ------------------------------------------------------------------ */
/* Compute A003961(m): replace each prime factor p^e by nextprime(p)^e */
/* ------------------------------------------------------------------ */
static seq_t A003961(uint64_t m)
{
    if (m <= 1)
        return (seq_t)1;

    uint64_t pr[64], ex[64];
    size_t cnt;

    factor_u64(m, pr, ex, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t base = (seq_t)next_prime(pr[i]);

        seq_t power = (seq_t)1;
        for (uint64_t e = 0; e < ex[i]; ++e)
            power *= base;

        result *= power;
    }

    return result;
}

/* ------------------------------------------------------------------ */
/* Recursive generator of all divisors from prime factorisation    */
/* ------------------------------------------------------------------ */
static void gen_divisors(const uint64_t *pr, const uint64_t *ex,
                         size_t pos, size_t cnt, uint64_t curr,
                         uint64_t *out, size_t *out_cnt)
{
    if (pos == cnt)
    {
        out[(*out_cnt)++] = curr;
        return;
    }

    uint64_t p = pr[pos];
    uint64_t val = 1;
    for (uint64_t e = 0; e <= ex[pos]; ++e)
    {
        gen_divisors(pr, ex, pos + 1, cnt, curr * val, out, out_cnt);
        val *= p;
    }
}

/* ------------------------------------------------------------------ */
/* Buffer reused for the list of divisors                            */
/* ------------------------------------------------------------------ */
static uint64_t *divs_buf = (uint64_t *)0;
static size_t divs_cap   = 0;

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Ensure buffer large enough for typical divisor count */
    const size_t need = 512;
    if (divs_cap < need)
    {
        free(divs_buf);
        divs_buf = (uint64_t *)malloc(need * sizeof(uint64_t));
        divs_cap = need;
    }

    /* Factor n */
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Generate divisors */
    size_t div_cnt = 0;
    gen_divisors(pr, ex, 0, cnt, 1, divs_buf, &div_cnt);

    /* Sum over divisors d | n of phi(d) * A003961(n/d) */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < div_cnt; ++i)
    {
        uint64_t d     = divs_buf[i];
        seq_t phi_d    = phi(d);
        seq_t val      = A003961((uint64_t)n / d);
        sum += phi_d * val;
    }

    return sum;
}

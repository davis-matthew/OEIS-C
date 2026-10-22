/*
 * A400872  The total number of exponential divisors of the exponential
 *          divisors of n.
 * https://oeis.org/A400872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy, new
 * Formula:   a(n) = ∏_{p^e‖n} S(e), where S(e) = Σ_{d|e} τ(d) (τ(d) is the
 *            number of ordinary divisors of d). A(p^e) = S(e)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Standard headers for integer types and size_t.  */
#include <stdint.h>
#include <stddef.h>

/*  Function prototypes are supplied by the build system.  */
seq_t a(idx_t n);

/*  Compute the ordinary divisor function τ(m) for m ≤ 64.  */
static seq_t tau_uint64(uint64_t m)
{
    seq_t res = 1;
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            unsigned cnt = 0;
            while (m % p == 0)
            {
                m /= p;
                ++cnt;
            }
            res *= (seq_t)(cnt + 1);
        }
    }
    if (m > 1)
        res *= 2;
    return res;
}

/*  Sum over the divisors of e of τ(d).  e is small (≤64).  */
static seq_t sum_tau_of_divisors(uint64_t e)
{
    seq_t sum = 0;
    for (uint64_t i = 1; i * i <= e; ++i)
    {
        if (e % i == 0)
        {
            sum += tau_uint64(i);
            if (i != e / i)
                sum += tau_uint64(e / i);
        }
    }
    return sum;
}

/*  Main sequence function: a(n).  Produces a(1)=1.  */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t x = (uint64_t)n;
    seq_t result = 1;

    /*  Factor n by trial division (works efficiently for n up to 10⁴
        and also for larger 64‑bit values).  Only exponents are needed.  */
    uint64_t exps[64];
    size_t count = 0;

    /*  Count factors of 2 first.  */
    unsigned exp = 0;
    while ((x & 1ULL) == 0ULL)
    {
        x >>= 1;
        ++exp;
    }
    if (exp > 0)
        exps[count++] = exp;

    /*  Then odd factors.  */
    for (uint64_t p = 3; p * p <= x; p += 2)
    {
        if (x % p == 0)
        {
            exp = 0;
            while (x % p == 0)
            {
                x /= p;
                ++exp;
            }
            exps[count++] = exp;
        }
    }
    /*  If there's a remaining prime factor > 1.  */
    if (x > 1)
        exps[count++] = 1;

    /*  Multiply the contribution of each prime power.  */
    for (size_t i = 0; i < count; ++i)
        result *= sum_tau_of_divisors(exps[i]);

    return result;
}

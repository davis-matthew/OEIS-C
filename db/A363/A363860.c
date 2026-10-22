/*
 * A363860  Number of divisors of 7*n-1 of form 7*k+4.
 * https://oeis.org/A363860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of divisors d of (7*n-1) with d ≡ 4 (mod 7)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors 7*n-1 using trial division and
   recursively enumerates its divisors, counting only those ≡ 4 (mod 7). */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Factor a uint64_t n into primes pr[] with corresponding exponents ex[].
   The number of distinct prime factors is stored in *cnt. */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p)
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

/* Recursively enumerate all divisors of a number given its prime factorisation.
   cur is the current divisor being formed.  Divisors congruent to 4 (mod 7)
   are counted in *cnt_res. */
static void enum_divs(const uint64_t *pr, const uint64_t *ex,
                      size_t pos, size_t cnt, uint64_t cur, seq_t *cnt_res)
{
    if (pos == cnt)
    {
        if ((cur % 7ULL) == 4ULL)
            (*cnt_res)++;
        return;
    }
    uint64_t val = cur;
    for (uint64_t e = 0; e <= ex[pos]; ++e)
    {
        enum_divs(pr, ex, pos + 1, cnt, val, cnt_res);
        if (e < ex[pos])
            val *= pr[pos];
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* outside the defined offset */

    uint64_t m = 7ULL * (uint64_t)n - 1ULL;

    /* factor 7*n-1 */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_uint64(m, pr, ex, &cnt);

    seq_t result = 0;
    enum_divs(pr, ex, 0, cnt, 1ULL, &result);
    return result;
}

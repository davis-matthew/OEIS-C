/*
 * A374466  a(n) = 1 if n is the product of an odd number of primes and the
 *          total number of 1-bits in the exponents of its prime factorization
 *          is odd, otherwise 0.
 * https://oeis.org/A374466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9909, u16 n <= 9909, u32 n <= 9909, u64 n <= 9909,
 *            u128 n <= 9909, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A374466
    a(n) = 1 if Ω(n) is odd and Σ popcount(e_i) is odd,
    where n = Π p_i^e_i is the prime factorization;
    otherwise 0. */

#include <stdint.h>
#include <stddef.h>

static int popcount_parity(uint64_t x)
{
    int p = 0;
    while (x)
    {
        p ^= 1;
        x &= x - 1;   /* remove lowest set bit */
    }
    return p;
}

/* External build types */
extern seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    uint64_t m = (uint64_t)n;
    /* A 64‑bit number has at most 64 distinct prime factors. */
    static const size_t MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt = 0;

    /* Trial division factorisation (fast enough for n ≤ 10⁴). */
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            uint64_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (m > 1)
    {
        primes[cnt] = m;
        exps[cnt]   = 1;
        ++cnt;
    }

    int parity_omega   = 0;  /* parity of Ω(n) */
    int parity_bits    = 0;  /* parity of Σ popcount(e_i) */

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = exps[i];
        parity_omega ^= (int)(e & 1ULL);          /* e mod 2 */
        parity_bits   ^= popcount_parity(e);      /* popcount(e) mod 2 */
    }

    return (seq_t)((parity_omega && parity_bits) ? 1 : 0);
}

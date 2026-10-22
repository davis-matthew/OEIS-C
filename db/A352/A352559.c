/*
 * A352559  a(n) = number of modules with n elements over the ring of integers
 *          in the real quadratic field of discriminant 33.
 * https://oeis.org/A352559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} g_p(e) where the local factor g_p(e) is: • p =
 *            3 or 11 (ramified) : g_p(e) = p(e) (partition number of e) • p
 *            splits in Q(√33) (Legendre (33|p)=1, p=2 included): g_p(e) =
 *            ∑_{a=0}^e p(a)·p(e−a) • p inert (Legendre (33|p)=−1): g_p(e) = 0
 *            if e is odd, g_p(e) = p(e/2) otherwise (p(k)...
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static bool init = false;
enum { MAX_E = 20, MAX_PRIME = 10000 };
static seq_t part[MAX_E];            /* partition numbers p(k) */
static seq_t conv[MAX_E];            /* convolution ∑ p(a)p(e−a) */
static uint64_t primes[2000];        /* list of primes up to MAX_PRIME */
static size_t prime_cnt = 0;

/* fast modular exponentiation for uint64_t, used for Legendre symbol */
static uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1;
    base %= mod;
    while (exp)
    {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

/* Legendre symbol (33|p), p odd prime not dividing 33 */
static int legendre_33(uint64_t p)
{
    /* p is assumed to be an odd prime > 3, 11 */
    uint64_t e = (p - 1) >> 1;
    uint64_t l = powmod64(33 % p, e, p);
    if (l == 1) return 1;          /* splits */
    if (l == p - 1) return -1;     /* inert */
    return 0;                      /* should not happen */
}

seq_t a(idx_t n)
{
    /* ----- pre‑compute constants on first call ----- */
    if (!init)
    {
        /* sieve for primes up to MAX_PRIME */
        bool comp[MAX_PRIME + 1] = {0};
        for (uint64_t i = 2; i <= MAX_PRIME; ++i)
        {
            if (!comp[i])
            {
                primes[prime_cnt++] = i;
                if (i * i <= MAX_PRIME)
                {
                    for (uint64_t j = i * i; j <= MAX_PRIME; j += i)
                        comp[j] = true;
                }
            }
        }

        /* partition numbers via the pentagonal theorem */
        part[0] = 1;
        for (int k = 1; k < MAX_E; ++k)
        {
            int64_t sum = 0;
            for (int m = 1; ; ++m)
            {
                int p1 = m * (3 * m - 1) / 2;
                int p2 = m * (3 * m + 1) / 2;
                int sgn = (m & 1) ? 1 : -1;
                if (p1 > k && p2 > k) break;
                if (p1 <= k) sum += sgn * (int64_t)part[k - p1];
                if (p2 <= k) sum += sgn * (int64_t)part[k - p2];
            }
            part[k] = (seq_t)sum;
        }

        /* convolution of partition numbers */
        for (int e = 0; e < MAX_E; ++e)
        {
            seq_t s = 0;
            for (int a = 0; a <= e; ++a)
                s += part[a] * part[e - a];
            conv[e] = s;
        }

        init = true;
    }

    /* ----- factorisation of n ----- */
    seq_t result = 1;
    uint64_t m = (uint64_t)n;          /* cast to 64‑bit for safety */

    for (size_t pi = 0; pi < prime_cnt && primes[pi] * primes[pi] <= m; ++pi)
    {
        uint64_t p = primes[pi];
        if (m % p == 0)
        {
            uint64_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }

            seq_t local = 0;
            if (p == 3 || p == 11)            /* ramified */
            {
                if (e < MAX_E) local = part[e];
            }
            else
            {
                int leg;
                if (p == 2) leg = 1;          /* 2 splits for D≡1 (mod 8) */
                else leg = legendre_33(p);

                if (leg == 1)                 /* split */
                {
                    if (e < MAX_E) local = conv[e];
                }
                else                           /* inert */
                {
                    if ((e & 1) == 0 && (e / 2) < MAX_E)
                        local = part[e >> 1];
                    else
                        local = 0;
                }
            }

            result *= local;
            if (result == 0) return 0;
        }
    }

    /* leftover prime factor > √n */
    if (m > 1)
    {
        uint64_t p = m;
        uint64_t e = 1;                   /* exponent is 1 */
        seq_t local = 0;

        if (p == 3 || p == 11)            /* ramified */
            local = part[1];
        else
        {
            int leg;
            if (p == 2) leg = 1;
            else leg = legendre_33(p);

            if (leg == 1) local = conv[1];
            else local = 0;                /* inert with e=1 gives 0 */
        }

        result *= local;
    }

    return result;
}

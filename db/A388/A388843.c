/*
 * A388843  Numbers k such that the factorial of k has fewer exponential
 *          divisors than the factorial of k+1.
 * https://oeis.org/A388843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A388843: a(n) = the nth integer k such that A388840(k) < A388840(k+1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- helper: divisor count of unsigned 64‑bit integer ---------- */
static uint64_t divisor_count_uint64(uint64_t x)
{
    if (x <= 1) return 1;          /* d(0) or d(1) treated as 1 */
    uint64_t d = 1;
    uint64_t p;
    for (p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            uint64_t e = 0;
            while (x % p == 0) { x /= p; ++e; }
            d *= (e + 1);
        }
    }
    if (x > 1) d *= 2;             /* remaining prime factor */
    return d;
}

/* ---------- helper: Euclidean GCD for 64‑bit integers ---------- */
static uint64_t uint64_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

/* ---------- main sequence routine ---------- */
seq_t a(idx_t n)
{
    /* static state shared across all calls */
    static bool  init      = false;
    static size_t term_cnt = 0, term_cap = 0;
    static seq_t *terms   = NULL;

    static uint64_t current_k = 0;      /* last processed k */
    static size_t   prime_cnt = 0, prime_cap = 0;
    static uint64_t *primes  = NULL;
    static uint64_t *exps    = NULL;    /* exponents of primes in current_k! */

    /* initialise storage */
    if (!init)
    {
        term_cap = 16;
        terms = (seq_t *)malloc(term_cap * sizeof(seq_t));

        prime_cap = 32;
        primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        exps   = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));

        init = true;
    }

    /* generate terms until we have at least n of them */
    while (term_cnt < (size_t)n)
    {
        uint64_t next_k = current_k + 1;
        uint64_t temp = next_k;

        /* fraction representing product of d(new)/d(old) for all prime factors of next_k */
        uint64_t num = 1, den = 1;

        /* factorise next_k using existing primes */
        size_t i = 0;
        while (i < prime_cnt && primes[i] * primes[i] <= temp)
        {
            uint64_t p = primes[i];
            if (temp % p == 0)
            {
                uint64_t v = 0;
                while (temp % p == 0) { temp /= p; ++v; }

                /* find or add prime p in the list */
                size_t idx = 0;
                while (idx < prime_cnt && primes[idx] != p) ++idx;
                if (idx == prime_cnt)
                {
                    if (prime_cnt == prime_cap)
                    {
                        prime_cap *= 2;
                        primes = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
                        exps   = (uint64_t *)realloc(exps,   prime_cap * sizeof(uint64_t));
                    }
                    primes[prime_cnt] = p;
                    exps[prime_cnt]   = 0;
                    idx = prime_cnt;
                    ++prime_cnt;
                }

                uint64_t e_old = exps[idx];
                uint64_t e_new = e_old + v;

                uint64_t d_old = divisor_count_uint64(e_old);
                uint64_t d_new = divisor_count_uint64(e_new);

                /* multiply fraction by d_new/d_old, reducing as we go */
                uint64_t g1 = uint64_gcd(d_new, d_old);
                d_new /= g1;
                d_old /= g1;

                num *= d_new;
                den *= d_old;

                uint64_t g2 = uint64_gcd(num, den);
                num /= g2;
                den /= g2;

                /* update exponent for this prime for future steps */
                exps[idx] = e_new;
            }
            ++i;
        }

        /* remaining prime factor > sqrt(temp) */
        if (temp > 1)
        {
            uint64_t p = temp;
            uint64_t v = 1;

            size_t idx = 0;
            while (idx < prime_cnt && primes[idx] != p) ++idx;
            if (idx == prime_cnt)
            {
                if (prime_cnt == prime_cap)
                {
                    prime_cap *= 2;
                    primes = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
                    exps   = (uint64_t *)realloc(exps,   prime_cap * sizeof(uint64_t));
                }
                primes[prime_cnt] = p;
                exps[prime_cnt]   = 0;
                idx = prime_cnt;
                ++prime_cnt;
            }

            uint64_t e_old = exps[idx];
            uint64_t e_new = e_old + v;

            uint64_t d_old = divisor_count_uint64(e_old);
            uint64_t d_new = divisor_count_uint64(e_new);

            uint64_t g1 = uint64_gcd(d_new, d_old);
            d_new /= g1;
            d_old /= g1;
            num *= d_new;
            den *= d_old;
            uint64_t g2 = uint64_gcd(num, den);
            num /= g2;
            den /= g2;

            exps[idx] = e_new;
        }

        /* if product of ratios > 1, record current_k as a term */
        if (num > den)
        {
            if (term_cnt == term_cap)
            {
                term_cap *= 2;
                terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
            }
            terms[term_cnt++] = (seq_t)current_k;
        }

        current_k = next_k;
    }

    return terms[n - 1];
}

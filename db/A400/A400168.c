/*
 * A400168  a(n) = A235224(A003415(n)) - A400164(n), where A235224 is the
 *          length of primorial base representation, A400164(n) = Max_{d|n, d
 *          is a prime power} A235224(n/d), and A003415 is the arithmetic
 *          derivative.
 * https://oeis.org/A400168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A235224(A003415(n)) – A400164(n) (the same as a(n) =
 *            A400262(n) – A400164(n))
 * Range:     u8 n <= 95, u16 n <= 9915, u32 n <= 9915, u64 n <= 9915,
 *            u128 n <= 9915, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

/* ------------------------------------------------------------
   Factor an integer n into prime powers.
   pr[0..cnt-1] and ex[0..cnt-1] are filled with base and exponent.
   ------------------------------------------------------------ */
static void factor(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t k = 0;
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
            pr[k] = p;
            ex[k] = e;
            ++k;
        }
    }
    if (n > 1)
    {
        pr[k] = n;
        ex[k] = 1;
        ++k;
    }
    *cnt = k;
}

/* ------------------------------------------------------------
   Arithmetic derivative of n (A003415).
   ------------------------------------------------------------ */
static seq_t arithmetic_derivative(uint64_t n)
{
    seq_t d = 0;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            uint64_t e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            d += (seq_t)e * (seq_t)(n / p);
        }
    }
    if (tmp > 1)
    {
        d += (seq_t)(n / tmp);
    }
    return d;
}

/* ------------------------------------------------------------
   Length of the primorial‑base representation (A235224).
   ------------------------------------------------------------ */
static seq_t primorial_length(seq_t m)
{
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;
    if (prime_list == NULL)
    {
        /* 200000 is enough for the range of n that will be asked. */
        const size_t limit = 200000;
        build_primes_upto(limit, &prime_list, &prime_cnt);
    }

    if (m < 2) return 0;          /* m = 0 or 1 => length 0 */

    seq_t prod = 1;
    size_t len = 0;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_list[i];
        seq_t next = prod * (seq_t)p;
        if (next > m) break;
        prod = next;
        ++len;
    }
    return (seq_t)len;
}

/* ------------------------------------------------------------
   Main sequence function.
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for 0, but harmless */

    /* Prime factorisation of n. */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor((uint64_t)n, pr, ex, &cnt);

    /* Arithmetic derivative of n. */
    seq_t der = arithmetic_derivative((uint64_t)n);
    seq_t len_der = primorial_length(der);

    /* A400164: maximum primorial length over prime‑power divisors. */
    seq_t max_len = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = pr[i];
        seq_t dp = 1;
        for (uint64_t k = 1; k <= ex[i]; ++k)
        {
            dp *= p;                           /* p^k */
            uint64_t n_div = (uint64_t)n / (uint64_t)dp;
            seq_t len = primorial_length(n_div);
            if (len > max_len) max_len = len;
        }
    }

    return len_der - max_len;
}

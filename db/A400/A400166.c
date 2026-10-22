/*
 * A400166  a(n) = A235224(n) - A400164(n), where A235224(n) is the length of
 *          primorial base representation of n, and A400164(n) = Max_{d|n, d
 *          is a prime power} A235224(n/d).
 * https://oeis.org/A400166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A235224(n) - A400164(n) where A235224(n) = number of
 *            digits in the primorial base representation of n and A400164(n)
 *            = max_{d|n,d prime power} A235224(n/d)
 * Range:     u8 n <= 9444, u16 n <= 9444, u32 n <= 9444, u64 n <= 9444,
 *            u128 n <= 9444, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --- Prime storage -------------------------------------------- */
static uint64_t *primes = NULL;   /* array of primes in ascending order   */
static uint64_t primes_cnt = 0;   /* number of primes stored              */
static uint64_t primes_cap = 0;   /* allocated capacity                   */
static uint64_t primes_last = 1;  /* last integer examined                */

/* Ensure the prime array contains all primes <= limit */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= primes_last)
        return;                     /* nothing new to do */

    for (uint64_t cand = primes_last + 1; cand <= limit; ++cand) {
        bool prime = true;
        /* test divisibility only with known primes up to sqrt(cand) */
        for (uint64_t j = 0; j < primes_cnt; ++j) {
            uint64_t p = primes[j];
            if (p > cand / p)               /* p*p > cand */
                break;
            if (cand % p == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {                       /* cand is prime */
            if (primes_cnt >= primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 8;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) abort();          /* allocation failed */
                primes = tmp;
            }
            primes[primes_cnt++] = cand;
        }
    }
    primes_last = limit;
}

/* --- Factorisation (trial division) -------------------------- */
static void factor_uint64(uint64_t n,
                          uint64_t *pf, uint64_t *exp,
                          size_t *cnt)
{
    uint64_t tmp = n;
    *cnt = 0;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            pf[(*cnt)] = p;
            exp[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (tmp > 1) {                       /* remaining prime factor */
        pf[(*cnt)] = tmp;
        exp[(*cnt)] = 1;
        ++(*cnt);
    }
}

/* --- Primorial base representation length ------------------ */
static seq_t primorial_len(uint64_t m)
{
    seq_t len = 1;        /* at least one digit */
    uint64_t prod = 1;

    /* we only need primes up to m, but product grows fast so limit is m */
    ensure_primes_upto(m);

    for (uint64_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (prod > m / p)          /* would exceed m */
            break;
        prod *= p;
        ++len;
    }
    return len;
}

/* --- Main sequence function ---------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;         /* undefined for n < 1 */

    uint64_t un = (uint64_t)n;

    /* A235224(n) */
    seq_t len_n = primorial_len(un);

    /* factor n to enumerate prime power divisors */
    uint64_t pf[64];
    uint64_t ex[64];
    size_t pf_cnt;
    factor_uint64(un, pf, ex, &pf_cnt);

    seq_t best = 0;

    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = pf[i];
        uint64_t e = ex[i];

        uint64_t pow = 1;
        for (uint64_t k = 1; k <= e; ++k) {
            pow *= p;                 /* p^k */
            uint64_t m = un / pow;
            seq_t len_m = primorial_len(m);
            if (len_m > best)
                best = len_m;
        }
    }

    return len_n - best;
}

/*
 * A398981  a(n) is the number k such that 2*(k*n+v) = Sum_{d|n} (k*d+v),
 *          where k and v are coprime and not both are 0.
 * https://oeis.org/A398981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (tau(n)-2)/gcd(tau(n)-2, 2*n-sigma(n))
 * Range:     i8 n <= 80, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A398981 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* ---   Prime sieve (one time, up to 100 000) ---------------------- */
/* ------------------------------------------------------------------ */
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t    prime_cnt  = 0;       /* number of primes in prime_list */

/* initialise prime_list once */
static void init_primes(void)
{
    if (prime_list != NULL) return;      /* already done */

    const size_t limit = 100000;         /* > sqrt(10^9) – plenty for n ≤ 10 000 */
    prime_cnt = 0;

    /* boolean array for sieve – use a byte per flag */
    uint8_t *barray = (uint8_t*)malloc(limit + 1);
    if (!barray) abort();                 /* out of memory – fatal */
    memset(barray, 0, limit + 1);

    for (size_t i = 2; i * i <= limit; ++i)
        if (!barray[i])
            for (size_t j = i * i; j <= limit; j += i)
                barray[j] = 1;

    /* count primes to allocate exactly enough space */
    for (size_t i = 2; i <= limit; ++i)
        if (!barray[i])
            ++prime_cnt;

    prime_list = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    if (!prime_list) abort();

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!barray[i])
            prime_list[idx++] = (uint64_t)i;

    free(barray);
}

/* ------------------------------------------------------------------ */
/* ---   Helper: absolute value, gcd ------------------------------- */
/* ------------------------------------------------------------------ */
static inline seq_t abs_seq_t(seq_t x)
{
    return (x < 0) ? -x : x;
}

/* Euclidean algorithm – works for all seq_t types (fixed or big integer) */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/* ---   Main sequence function ------------------------------------- */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* defined only for n ≥ 1; guard against accidental 0 */
    if (n == 0) return (seq_t)0;

    init_primes();                     /* ensure prime list ready */

    /* Factorisation – compute tau(n) and sigma(n) */
    idx_t  nn   = n;                    /* mutable copy for factorisation */
    seq_t  tau  = 1;                     /* number of divisors */
    seq_t  sigma = 1;                    /* sum of divisors */

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > (uint64_t)nn) break;
        if (nn % p == 0)
        {
            idx_t e = 0;
            while (nn % p == 0) { nn /= p; ++e; }

            /* tau multiplies by (e+1) */
            tau *= (seq_t)(e + 1);

            /* compute p^(e+1) */
            seq_t term = 1;
            for (idx_t k = 0; k < e + 1; ++k)
                term *= (seq_t)p;          /* term = p^(e+1) */

            /* contribution of this prime power to sigma */
            seq_t mult = (term - 1) / ((seq_t)p - 1);    /* (p^(e+1)-1)/(p-1) */
            sigma *= mult;
        }
    }

    /* Any remaining prime factor > sqrt(original n) */
    if (nn > 1)
    {
        tau   *= 2;                           /* exponent 1 → factor (1+1) */
        sigma *= (seq_t)(1 + (seq_t)nn);
    }

    /* ---------------------------------------------------------------- */
    /* ---   Compute a(n) = (tau-2)/gcd(tau-2, 2*n - sigma) --------- */
    /* ---------------------------------------------------------------- */
    seq_t num  = tau - 2;                         /* τ(n) – 2   */
    seq_t diff = (seq_t)n * 2 - sigma;              /* 2*n – σ(n) */

    seq_t d = gcd_seq_t(abs_seq_t(num), abs_seq_t(diff)); /* gcd of absolute values */

    if (d == 0)    /* safety – this should never happen */
        return num;

    return num / d;
}

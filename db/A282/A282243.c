/*
 * A282243  Square root of the smallest square referenced in A049094 (Numbers
 *          n such that 2^n - 1 is divisible by a square > 1).
 * https://oeis.org/A282243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 296, u32 n <= 296, u64 n <= 296,
 *            u128 n <= 296, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A282243
 *  a(n) = sqrt of the smallest square >1 dividing 2^k-1
 *  where k is the nth element of A049094
 *  (numbers n such that 2^n-1 has a non‑trivial square divisor).
 *
 *  The implementation indexes the sequence as a(1)=first term.
 *  It precomputes all terms up to a fixed maximal exponent (200000),
 *  which is more than enough to answer any request for
 *  indices up to about 100 000.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* -------------- Utility: modular exponentiation for 64‑bit modulus ------------- */
static inline uint64_t modpow2(uint64_t base, uint64_t exp, uint64_t mod)
{
    __uint128_t res = 1;
    __uint128_t b   = base % mod;
    while (exp) {
        if (exp & 1) res = (res * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (uint64_t)res;
}

/* -------------- Constants and static storage ---------------------------------- */
#define MAX_E 500000             /* maximal exponent considered */

static uint32_t spf[MAX_E + 1];           /* smallest prime factor */
static uint32_t primes[MAX_E / 10 + 1];   /* list of primes up to MAX_E */
static size_t  prime_cnt = 0;

/* -------------- a() function -------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static data that is initialised on the first call */
    static bool  init_done = false;
    static uint32_t bestPrime[MAX_E + 1];   /* bestPrime[e] = p where p^2 | 2^e-1 and
                                              p is minimal; 0 if no such prime yet */
    static seq_t *seqVals = NULL;          /* vector of sequence values */
    static size_t seqSize = 0;

    if (!init_done) {
        /* 1. Linear sieve to compute spf[] and primes[] */
        memset(spf, 0, sizeof(spf));
        for (uint32_t i = 2; i <= MAX_E; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes[prime_cnt++] = i;
            }
            for (size_t j = 0; j < prime_cnt &&
                 primes[j] <= spf[i] && (uint64_t)i * primes[j] <= MAX_E; ++j) {
                spf[i * primes[j]] = primes[j];
            }
        }

        /* 2. Mark bestPrime for each exponent */
        memset(bestPrime, 0, sizeof(bestPrime));

        for (size_t pi = 1; pi < prime_cnt; ++pi) {   /* skip prime 2 */
            uint32_t p = primes[pi];
            uint64_t mod = (uint64_t)p * (uint64_t)p;
            uint64_t phi = (uint64_t)p * (p - 1);
            uint64_t t = phi;

            /* collect distinct prime factors of phi (=p*(p-1)) */
            uint32_t fac[32];
            size_t fc = 0;
            fac[fc++] = p;                 /* factor p itself */

            uint32_t x = p - 1;
            while (x > 1) {
                uint32_t f = spf[x];
                bool exists = false;
                for (size_t k = 0; k < fc; ++k)
                    if (fac[k] == f) { exists = true; break; }
                if (!exists) fac[fc++] = f;
                while (x % f == 0) x /= f;
            }

            /* reduce t to the multiplicative order of 2 modulo p^2 */
            for (size_t fi = 0; fi < fc; ++fi) {
                uint32_t f = fac[fi];
                while (t % f == 0) {
                    uint64_t tdiv = t / f;
                    if (modpow2(2, tdiv, mod) == 1)
                        t = tdiv;
                    else
                        break;
                }
            }
            uint32_t ord = (uint32_t)t;    /* ord <= MAX_E */

            if (ord > MAX_E) continue;     /* will never hit exponents <= MAX_E */

            /* mark all multiples of ord */
            for (uint32_t m = ord; m <= MAX_E; m += ord)
                if (bestPrime[m] == 0)
                    bestPrime[m] = p;
        }

        /* 3. Build the sequence vector seqVals */
        seqVals = NULL;
        seqSize = 0;
        size_t seqCap = 0;

        for (uint32_t e = 1; e <= MAX_E; ++e) {
            uint32_t p = bestPrime[e];
            if (p == 0) continue;                /* exponent e has no square divisor */
            if (seqSize == seqCap) {
                seqCap = seqCap ? seqCap * 2 : 1024;
                seqVals = (seq_t *)realloc(seqVals,
                                            seqCap * sizeof(seq_t));
            }
            seqVals[seqSize++] = (seq_t)p;        /* cast to seq_t */
        }

        init_done = true;
    }

    /* 4. Answer the query */
    if (n == 0 || n > seqSize)
        return 0;                /* undefined for out of range; safety return */
    return seqVals[n - 1];       /* 1‑based indexing */
}

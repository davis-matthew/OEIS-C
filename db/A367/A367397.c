/*
 * A367397  Numbers m such that bigomega(m) is the sum of prime indices of
 *          some semiprime divisor of m.
 * https://oeis.org/A367397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A367397: Numbers m such that bigomega(m) equals the sum of prime
   indices of some semiprime divisor of m.  The routine builds a dynamic
   sieve the first time it is called and caches all sequence members for
   subsequent look‑ups.  It requires the helper bigomega from the utils
   directory. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/bigomega.h"

/* global state */
static bool initialized = false;
static idx_t limit = 1000;          /* current sieve limit */
static uint32_t *spf = NULL;        /* smallest prime factor for each number */
static uint32_t *primes = NULL;     /* list of primes, 1‑based indexing */
static int *primeIdx = NULL;        /* prime index: primeIdx[p] = k for k‑th prime */
static size_t primesCnt = 0;

static seq_t *seqVals = NULL;       /* cached sequence values */
static size_t seqCap = 0;           /* capacity of seqVals */
static size_t seqSize = 0;          /* number of stored terms */
static idx_t lastChecked = 1;       /* last integer that was examined */

/* ------------------------------------------------------------------ */
/* Build a linear sieve up to 'up'.  The arrays spf, primes, and
   primeIdx are allocated (or re‑allocated) to fit [0..up]. */
static void build_sieve(idx_t up)
{
    if (spf) free(spf);
    if (primeIdx) free(primeIdx);
    if (primes) free(primes);

    spf = (uint32_t*)calloc((size_t)(up + 1), sizeof(uint32_t));
    primeIdx = (int*)calloc((size_t)(up + 1), sizeof(int));
    /* allocate at most up primes */
    primes = (uint32_t*)malloc((size_t)(up + 1) * sizeof(uint32_t));

    size_t pc = 0;
    for (idx_t i = 2; i <= up; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = (uint32_t)i;
            primeIdx[i] = (int)(++pc);
            primes[pc - 1] = (uint32_t)i;
        }
        for (size_t j = 0; j < pc; ++j) {
            uint32_t p = primes[j];
            idx_t prod = (idx_t)p * i;
            if (prod > up) break;
            if (spf[prod] == 0) spf[prod] = p;
            if (p == spf[i]) break;        /* avoid duplicate work */
        }
    }
    primesCnt = pc;
    limit = up;
}

/* ------------------------------------------------------------------ */
/* Ensure that we have computed all sequence elements up to index n
   (1‑based).  If needed, the sieve limit is increased, and we
   continue scanning integers until enough terms have been found. */
static void ensure_seq(idx_t n)
{
    if (!initialized) {                     /* first exercise */
        build_sieve(limit);
        seqCap = 64;
        seqVals = (seq_t*)malloc(seqCap * sizeof(seq_t));
        seqSize = 0;
        lastChecked = 1;
        initialized = true;
    }

    while (seqSize < n) {
        ++lastChecked;

        /* factor lastChecked using spf, collect prime indices & exponents */
        idx_t tmp = lastChecked;
        int facIdx[32];
        int facExp[32];
        size_t facCnt = 0;
        while (tmp > 1) {
            uint32_t p = spf[(size_t)tmp];
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            facIdx[facCnt] = primeIdx[p];
            facExp[facCnt] = e;
            ++facCnt;
        }

        seq_t target = bigomega(lastChecked);
        bool match = false;

        /* check all unordered pairs of prime factors (allowing equal primes)
           that could form a semiprime divisor of lastChecked. */
        for (size_t i = 0; i < facCnt && !match; ++i) {
            /* semiprime p*p: requires exponent >= 2 */
            if (facExp[i] >= 2) {
                seq_t s = (seq_t)facIdx[i] * 2;
                if (s == target) match = true;
            }
            /* semiprime p*q with distinct primes */
            for (size_t j = i + 1; j < facCnt && !match; ++j) {
                seq_t s = (seq_t)facIdx[i] + (seq_t)facIdx[j];
                if (s == target) match = true;
            }
        }

        if (match) {
            /* store the term */
            if (seqSize == seqCap) {
                seqCap *= 2;
                seqVals = (seq_t*)realloc(seqVals, seqCap * sizeof(seq_t));
            }
            seqVals[seqSize++] = (seq_t)lastChecked;
        }

        /* If lastChecked grows beyond current sieve limit, double the limit
           and rebuild the sieve to handle further factorisations. */
        if (lastChecked > limit) {
            idx_t newlim = limit * 2;
            if (newlim < lastChecked) newlim = lastChecked * 2;
            build_sieve(newlim);
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public entry point required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, zero is safe */
    ensure_seq(n);
    return seqVals[n - 1];
}

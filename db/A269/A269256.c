/*
 * A269256  Chen primes p such that there are Chen primes p > q > r in
 *          arithmetic progression.
 * https://oeis.org/A269256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 2969, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A269256: Chen primes that appear as the largest term of a 3‑term
   arithmetic progression of Chen primes.
   The code generates primes up to a fixed limit, pre‑computes Chen primes,
   and then scans those to collect the desired subsequence.  It is designed
   to return the nth term for any  n ≥ 1   (offset 1).  The limit is chosen
   large enough that 10 000 terms can be produced in well under two seconds. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* upper bound for prime generation – chosen to contain >10 000 valid terms */
#define MAX_PRIME_LIMIT 40000000

/* static storage for the sieve, Chen flag, prime lists and the result */
static uint8_t *isPrime  = NULL;   /* 0/1 flag for each number */
static uint8_t *isChen   = NULL;   /* 0/1 flag for Chen primes */
static uint32_t *primes  = NULL;   /* list of all primes */
static size_t    primesCnt = 0;
static uint32_t *chenPrimes = NULL;/* list of Chen primes */
static size_t    chenCnt = 0;
static uint32_t *seqData = NULL;   /* sequence A269256 */
static size_t    seqCnt = 0;
static bool      initialized = false;

/* simple sieve of Eratosthenes up to MAX_PRIME_LIMIT */
static void init_sieve(void)
{
    size_t i, j;
    size_t limit = MAX_PRIME_LIMIT;
    isPrime = (uint8_t*)malloc(limit + 1);
    isChen  = (uint8_t*)malloc(limit + 2); /* need index p+2 for Chen test */
    if (!isPrime || !isChen) abort();

    for (i = 0; i <= limit; ++i) isPrime[i] = 1;
    isPrime[0] = isPrime[1] = 0;

    for (i = 2; i * i <= limit; ++i)
        if (isPrime[i])
            for (j = i * i; j <= limit; j += i)
                isPrime[j] = 0;

    /* gather all primes first */
    primesCnt = 0;
    for (i = 2; i <= limit; ++i)
        if (isPrime[i]) ++primesCnt;
    primes = (uint32_t*)malloc(primesCnt * sizeof(uint32_t));
    if (!primes) abort();
    for (i = 2, j = 0; i <= limit; ++i)
        if (isPrime[i]) primes[j++] = (uint32_t)i;

    /* mark Chen primes:  p is prime and p+2 is prime or semiprime */
    chenCnt = 0;
    for (i = 0; i < primesCnt; ++i) {
        uint32_t p = primes[i];
        /* test whether p+2 is prime or a product of two primes */
        bool good = false;
        uint32_t n = p + 2;
        if (isPrime[n]) good = true;
        else {
            /* test if n is semiprime */
            bool semiprime = false;
            for (size_t k = 0; k < primesCnt && (uint64_t)primes[k] * primes[k] <= n; ++k) {
                uint32_t q = primes[k];
                if (n % q == 0) {
                    uint32_t t = n / q;
                    if (isPrime[t]) { semiprime = true; break; }
                    else break; /* more than two prime factors */
                }
            }
            good = semiprime;
        }
        if (good) ++chenCnt;
    }

    chenPrimes = (uint32_t*)malloc(chenCnt * sizeof(uint32_t));
    if (!chenPrimes) abort();
    for (i = 0, j = 0; i < primesCnt; ++i) {
        uint32_t p = primes[i];
        uint32_t n = p + 2;
        bool good = false;
        if (isPrime[n]) good = true;
        else {
            for (size_t k = 0; k < primesCnt && (uint64_t)primes[k] * primes[k] <= n; ++k) {
                uint32_t q = primes[k];
                if (n % q == 0) {
                    uint32_t t = n / q;
                    if (isPrime[t]) { good = true; break; }
                    else break;
                }
            }
        }
        if (good) {
            chenPrimes[j++] = p;
            isChen[p] = 1;
        } else {
            isChen[p] = 0;
        }
    }

    /* compute the sequence A269256 */
    const size_t MAX_SEQ_SIZE = 20000; /* safe upper bound for 10 000 terms */
    seqData = (uint32_t*)malloc(MAX_SEQ_SIZE * sizeof(uint32_t));
    if (!seqData) abort();

    for (size_t idx = 0; idx < chenCnt; ++idx) {
        uint32_t p = chenPrimes[idx];
        uint32_t half = (p + 1) / 2;
        /* find first chenPrimes[j] > half (binary search) */
        size_t lo = 0, hi = idx;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (chenPrimes[mid] <= half) lo = mid + 1; else hi = mid;
        }
        bool found = false;
        for (size_t j = lo; j < idx; ++j) {
            uint32_t q = chenPrimes[j];
            uint32_t r = 2 * q - p;
            if (r <= MAX_PRIME_LIMIT && isChen[r]) { found = true; break; }
        }
        if (found) {
            seqData[seqCnt++] = p;
            if (seqCnt == MAX_SEQ_SIZE) break;
        }
    }
    initialized = true;
}

/* public function required by the build */
seq_t a(idx_t n)
{
    if (!initialized) init_sieve();
    if (n == 0 || n > seqCnt) return 0; /* out of range – should not happen */
    return (seq_t)seqData[(size_t)(n - 1)];
}

/*
 * A354834  Numbers k such that 2*k can be written in at least one way as p+q
 *          with p, q, p+2*q and 2*q+p all prime.
 * https://oeis.org/A354834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A354834: a(n) = k such that 2*k can be expressed as p + q
 * with primes p, q, p + 2*q and 2*p + q all prime.
 *
 * Computed by brute‑force search using a global prime sieve and a
 * persistent cache of found terms. The algorithm generates k in
 * increasing order until the required n is reached.  The sieve size
 * (5 000 000) is sufficient for the first 10 000 terms. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* ---------- static state ------------------------------------ */
    static bool initialized = false;
    static const size_t SIEVE_LIMIT = 5000000;
    static unsigned char *isPrime = NULL;   /* 0: not prime, 1: prime */
    static uint32_t *primes = NULL;          /* list of primes */
    static size_t primes_cnt = 0;
    static seq_t *cache = NULL;              /* cached terms */
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static size_t next_k = 1;                /* next k to test */

    /* ---------- one‑time initialization ------------------------ */
    if (!initialized) {
        /* build sieve */
        isPrime = (unsigned char *)malloc((SIEVE_LIMIT + 1) * sizeof(unsigned char));
        if (!isPrime) exit(1);
        memset(isPrime, 1, SIEVE_LIMIT + 1);
        isPrime[0] = isPrime[1] = 0;
        for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
            if (isPrime[i]) {
                for (size_t j = i * i; j <= SIEVE_LIMIT; j += i) isPrime[j] = 0;
            }
        }
        /* collect primes */
        primes_cnt = 0;
        for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
            if (isPrime[i]) ++primes_cnt;
        primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
        if (!primes) exit(1);
        size_t idx = 0;
        for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
            if (isPrime[i]) primes[idx++] = (uint32_t)i;

        /* allocate cache */
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache_size = 0;
        initialized = true;
    }

    /* ---------- generate terms until we have enough ---------------- */
    while (cache_size < n) {
        /* find next qualifying k */
        for (;;) {
            uint64_t k = next_k++;
            uint64_t S = k << 1;                /* 2*k   */
            bool ok = false;

            /* iterate primes p <= S/2 */
            for (size_t pi = 0; pi < primes_cnt; ++pi) {
                uint32_t p = primes[pi];
                if ((uint64_t)p > S / 2) break;

                uint64_t q = S - p;              /* q = S - p  */
                if (q > SIEVE_LIMIT || !isPrime[(size_t)q]) continue;

                uint64_t x1 = p + 2 * q;          /* p + 2*q   */
                uint64_t x2 = 2ULL * p + q;       /* 2p + q    */
                if (x1 > SIEVE_LIMIT || x2 > SIEVE_LIMIT) continue;
                if (isPrime[(size_t)x1] && isPrime[(size_t)x2]) {
                    ok = true;
                    break;
                }
            }

            if (ok) {
                /* store found k */
                if (cache_size == cache_cap) {
                    cache_cap <<= 1;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!cache) exit(1);
                }
                cache[cache_size++] = (seq_t)k;
                break;                     /* go to outer loop */
            }
        }
    }

    return cache[n - 1];
}

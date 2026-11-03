/*
 * A341421  a(n) is the number of k <= n for which (prime(k)+3*prime(k+1))/2
 *          is prime minus the number of k <= n for which
 *          (prime(k)+3*prime(k+2))/2 is prime.
 * https://oeis.org/A341421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = #{k ≤ n : (prime(k)+3*prime(k+1))/2 is prime} - #{k ≤ n :
 *            (prime(k)+3*prime(k+2))/2 is prime}
 * Range:     i8 n <= 10, i16 n <= 2678, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9559 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses a simple sieve to obtain the first n+2 primes and tests
 * each candidate value only when the numerator is even, thus avoiding
 * false positives caused by integer division of odd sums. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <string.h>
#include <math.h>

/* --- prime generation using a single sieve ------------------------------------------------- */
static void sieve_first_n_primes(idx_t n, uint32_t **primes_out, size_t *cnt_out)
{
    /* Upper bound for the n-th prime (for n >= 6): p_n < n(ln n + ln ln n) */
    idx_t limit;
    if (n < 6) {
        limit = 15;
    } else {
        double dn = (double)n;
        double upper = dn * (log(dn) + log(log(dn))) + 3.0 * dn; /* generous safety */
        limit = (idx_t)upper + 10;
    }

    /* Allocate space for collecting primes */
    size_t est_capacity = (size_t)(n + 16);
    uint32_t *primes = (uint32_t *)malloc(est_capacity * sizeof(uint32_t));
    size_t primes_cnt = 0;

    /* Sieve of Eratosthenes */
    unsigned char *is_prime = (unsigned char *)malloc((limit + 1) * sizeof(unsigned char));
    memset(is_prime, 1, (limit + 1));
    is_prime[0] = is_prime[1] = 0;
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= (uint64_t)limit; j += i)
                is_prime[j] = 0;
        }
    }
    for (uint32_t i = 2; i <= (uint32_t)limit && primes_cnt < (size_t)n; ++i) {
        if (is_prime[i]) {
            if (primes_cnt == est_capacity) {
                est_capacity <<= 1;
                primes = (uint32_t *)realloc(primes, est_capacity * sizeof(uint32_t));
            }
            primes[primes_cnt++] = i;
        }
    }

    free(is_prime);
    /* In the extremely unlikely event we did not obtain enough primes, increase limit and retry */
    while (primes_cnt < (size_t)n) {
        limit <<= 1;                               /* double the limit */
        free(primes);
        est_capacity = (size_t)(n + 16);
        primes = (uint32_t *)malloc(est_capacity * sizeof(uint32_t));
        primes_cnt = 0;

        is_prime = (unsigned char *)malloc((limit + 1) * sizeof(unsigned char));
        memset(is_prime, 1, (limit + 1));
        is_prime[0] = is_prime[1] = 0;
        for (uint64_t i = 2; i * i <= limit; ++i) {
            if (is_prime[i]) {
                for (uint64_t j = i * i; j <= (uint64_t)limit; j += i)
                    is_prime[j] = 0;
            }
        }
        for (uint32_t i = 2; i <= (uint32_t)limit && primes_cnt < (size_t)n; ++i) {
            if (is_prime[i]) {
                if (primes_cnt == est_capacity) {
                    est_capacity <<= 1;
                    primes = (uint32_t *)realloc(primes, est_capacity * sizeof(uint32_t));
                }
                primes[primes_cnt++] = i;
            }
        }
        free(is_prime);
    }

    *primes_out = primes;
    *cnt_out = primes_cnt;
}

static bool isPrime128(__uint128_t n)
{
    if (n < 2) return false;

    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* Deterministic bases for 128‑bit integers (covers < 2^128) */
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
                                     19ULL,23ULL,29ULL,31ULL,37ULL};

    uint64_t d = (uint64_t)(n - 1);
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        __uint128_t x = 1, pw = d, base = a;
        while (pw) {
            if (pw & 1ULL) x = (x * base) % n;
            base = (base * base) % n;
            pw >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* -----------------  Helper:  Miller–Rabin for 64‑bit ----------------- */
static bool isPrimeULL(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0) return n == 2ULL;

    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL,
                                     28178ULL, 450775ULL, 9780504ULL,
                                     1795265022ULL};
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, pw = d, base = a;
        while (pw) {
            if (pw & 1ULL) x = (x * base) % n;
            base = (base * base) % n;
            pw >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return isPrimeULL((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return isPrime128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

static uint32_t *primes = NULL;     /* 1‑based indexing: primes[0] = prime(1) */
static size_t   primes_cnt = 0;     /* number of stored primes */

/* Return a(n) for the sequence A341421 */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* undefined for n < 1, but sub‑write 0 */

    /* We need primes up to prime(n+2) → n+2 primes */
    idx_t needed = n + 2;
    if (needed > (idx_t)primes_cnt) {
        if (primes)
            free(primes);
        primes = NULL;
        /* sieve_first_n_primes expects the count of primes, not the limit. */
        sieve_first_n_primes(needed, &primes, &primes_cnt);   /* primes[0] = prime(1) */
    }

    seq_t diff = 0;  /* running difference of counts */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t p_k   = (seq_t)primes[k-1];
        seq_t p_k1  = (seq_t)primes[k];     /* prime(k+1) */
        seq_t p_k2  = (seq_t)primes[k+1];   /* prime(k+2) */

        seq_t s1 = p_k + (seq_t)3 * p_k1;
        if ((s1 & 1) == 0) {                  /* even sum → integer candidate */
            seq_t val1 = s1 >> 1;             /* divide by 2 using shift */
            if (isPrimeGeneric(val1))
                ++diff;
        }

        seq_t s2 = p_k + (seq_t)3 * p_k2;
        if ((s2 & 1) == 0) {
            seq_t val2 = s2 >> 1;
            if (isPrimeGeneric(val2))
                --diff;
        }
    }
    return diff;
}

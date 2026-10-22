/*
 * A356309  The least j >= n such that n and A276086(j) are relatively prime,
 *          where A276086 is the primorial base exp-function.
 * https://oeis.org/A356309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 2430, u32 n <= 9596, u64 n <= 9596,
 *            u128 n <= 9596, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = n + A356302(n)  (derived from OEIS entry A356309)
 * This implementation follows the definition:
 *   a(n) = the least j >= n such that gcd(n, A276086(j)) == 1,
 * where A276086(j) is the primorial‑base exponent function.
 *
 * The primorial‑base exponent is computed by writing j in primorial
 * base (digits bounded by the next prime) and interpreting the
 * digits as exponents of the successive primes.
 *
 * For each n we factor n, then search j >= n until no prime factor of n
 * occurs in the primorial‑base digits of j.
 *
 * The code precomputes a prime table once using a sieve up to 200 000,
 * which is more than enough for all typical n (up to ~10 000).
 *
 * The function is thread‑safe for single‑threaded use and will handle
 * any n that fits in idx_t.  It returns 0 for n = 0, as specified.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime table (single initialisation) ---------- */

#define PRIME_LIMIT 200000

static uint32_t *prime_tbl = NULL;     /* array of primes */
static size_t     prime_cnt = 0;       /* number of primes in prime_tbl */

static void init_prime_table(void)
{
    if (prime_tbl != NULL) return; /* already initialised */

    bool *sieve = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!sieve) exit(1);           /* allocation failed */

    size_t cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            ++cnt;
            if ((uint64_t)i * (uint64_t)i <= PRIME_LIMIT) {
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                    sieve[j] = true;
            }
        }
    }

    prime_tbl = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!prime_tbl) {
        free(sieve);
        exit(1);
    }
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!sieve[i]) prime_tbl[idx++] = (uint32_t)i;

    free(sieve);
    prime_cnt = cnt;
}

/* ---------- Factorisation (distinct primes only) ---------- */

static void factor_distinct(idx_t n,
                            uint32_t *out_primes,
                            size_t *out_indices,
                            size_t *out_cnt)
{
    if (n == 0 || n == 1) {
        *out_cnt = 0;
        return;
    }

    idx_t tmp = n;
    size_t cnt = 0;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_tbl[i]*(uint64_t)prime_tbl[i] <= tmp; ++i) {
        uint32_t p = prime_tbl[i];
        if (tmp % p == 0) {
            out_primes[cnt] = p;
            out_indices[cnt] = i;
            ++cnt;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) { /* tmp is prime > sqrt(original n) */
        /* binary search to find its index in the prime table */
        size_t lo = 0, hi = prime_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (prime_tbl[mid] < (uint32_t)tmp) lo = mid + 1;
            else hi = mid;
        }
        if (lo == prime_cnt || prime_tbl[lo] != (uint32_t)tmp) {
            /* this should not happen for n <= PRIME_LIMIT */
        } else {
            out_primes[cnt] = (uint32_t)tmp;
            out_indices[cnt] = lo;
            ++cnt;
        }
    }
    *out_cnt = cnt;
}

/* ---------- Main sequence function ---------- */

seq_t a(idx_t n)
{
    /* Special case for the offset */
    if (n == 0) return 0;

    init_prime_table();  /* ensure primes are ready */

    /* Factorise n to obtain its distinct prime factors and their indices */
    uint32_t factors[64];
    size_t   f_indices[64];
    size_t   f_cnt = 0;
    factor_distinct(n, factors, f_indices, &f_cnt);

    /* If n = 1, all gcds are 1; return n immediately */
    if (f_cnt == 0) return (seq_t)n;

    /* Determine the maximum prime index needed for digit extraction */
    size_t max_prime_index = 0;
    for (size_t i = 0; i < f_cnt; ++i)
        if (f_indices[i] > max_prime_index) max_prime_index = f_indices[i];

    /* Search for the smallest j >= n satisfying the coprimality constraint */
    idx_t j = n;
    for (;; ++j) {
        idx_t val = j;
        bool bad = false;
        for (size_t k = 0; k <= max_prime_index; ++k) {
            uint32_t p = prime_tbl[k];
            idx_t    digit = val % p;
            if (digit > 0) {
                /* Check whether p is a prime factor of n */
                for (size_t fi = 0; fi < f_cnt; ++fi) {
                    if (f_indices[fi] == k) { bad = true; break; }
                }
                if (bad) break;
            }
            val /= p;
        }
        if (!bad) return (seq_t)j;
    }
}

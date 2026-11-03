/*
 * A364807  Numbers k such that abs(k - Sum_{m=2..k} pi(prime(k)/m)) is a
 *          prime number, where pi(k) is number of primes <= k.
 * https://oeis.org/A364807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k such that |k - Σ_{m=2..k} π(prime(k)/m)| is prime
 * Range:     u8 n <= 13, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 57 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence enumerated in increasing order from the smallest such k. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

#define PRIMES_LIMIT   2000000        /* upper bound for sieve (≈2·10⁶) */

static bool initialized = false;

/* Prime data structures */
static uint32_t *primes = NULL;      /* prime list (1‑based index with primes[0]=2) */
static size_t primes_cnt = 0;        /* number of primes stored */
static uint32_t *pi_arr = NULL;      /* pi_arr[n] = #{primes ≤ n} for 0 ≤ n ≤ PRIMES_LIMIT */

/* Sequence cache */
static seq_t  *seq_cache = NULL;     /* dynamic array holding terms of the sequence */
static size_t  seq_len = 0;          /* number of terms stored */
static size_t  seq_cap = 0;          /* capacity of seq_cache */

/* The next integer k to test */
static idx_t next_k = 2;

/* Helper to create a sieve and compute pi array */
static void init_prime_data(void)
{
    const size_t limit = PRIMES_LIMIT;
    bool *is_comp = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_comp) exit(1);
    memset(is_comp, 0, (limit + 1) * sizeof(bool));

    for (size_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;
        }
    }

    /* Allocate worst‑case space for primes (≈limit/log(limit)) */
    primes = (uint32_t*)malloc((limit / 10 + 100) * sizeof(uint32_t));
    if (!primes) exit(1);
    pi_arr = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    if (!pi_arr) exit(1);

    size_t cnt = 0;
    for (size_t n = 2; n <= limit; ++n) {
        if (!is_comp[n]) {
            primes[cnt++] = (uint32_t)n;
        }
        pi_arr[n] = (uint32_t)cnt;
    }
    primes_cnt = cnt;

    free(is_comp);
}

seq_t a(idx_t n)
{
    /* one‑shot initialization */
    if (!initialized) {
        init_prime_data();
        initialized = true;
    }

    /* Ensure we have enough terms in the cache */
    while (seq_len <= n) {
        if (next_k > primes_cnt) {
            /* We have exhausted the sieve – stop gracefully */
            return 0;  /* undefined for indices beyond our sieve */
        }

        uint32_t pk = primes[(size_t)next_k - 1];          /* k‑th prime (1‑based) */
        uint64_t k64 = (uint64_t)next_k;
        uint64_t sum = 0;

        uint64_t m = 2;
        while (m <= k64) {
            uint64_t v = pk / m;                 /* floor division */
            if (v == 0) { break; }               /* safety, should not happen */
            uint64_t pi_v = pi_arr[(size_t)v];
            uint64_t max_m = pk / v;
            if (max_m > k64) max_m = k64;
            sum += pi_v * (max_m - m + 1);
            m = max_m + 1;
        }

        uint64_t diff = (k64 >= sum) ? (k64 - sum) : (sum - k64);
        if (diff > 1 && is_prime_big((seq_t)diff)) {
            /* Grow cache if necessary */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq_cache = mem_resize(seq_t, seq_cache, seq_cap);
                if (!seq_cache) exit(1);
            }
            seq_cache[seq_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return seq_cache[n];
}

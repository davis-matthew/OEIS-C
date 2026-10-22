/*
 * A377051  Array read by antidiagonals downward where A(n,k) is the n-th term
 *          of the k-th differences of the powers of primes.
 * https://oeis.org/A377051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n-th term of the array read by antidiagonals *
 *            downward of A(i,j) = Σ_{k=0..j} (-1)^(j-k) * binomial(j,k) *
 *            A000961(i+k). * A000961 is the sequence of prime powers
 *            (1,2,3,4,…)
 * Range:     i8 none, i16 none, i32 none, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- prime‑power cache ------------------------------------- */
#define PRIME_POWER_CACHE_CAP 4096

static seq_t prime_power_cache[PRIME_POWER_CACHE_CAP];
static size_t prime_power_len = 0;

/* ---------- combinatorial helpers --------------------------------- */
static inline seq_t binom(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r *= (seq_t)(n - k + i);
        r /= (seq_t)i;               /* division is exact */
    }
    return r;
}

/* ---------- compute A(i,j) --------------------------------------- */
static seq_t A_ij(idx_t i, idx_t j)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= j; ++k) {
        seq_t coeff = binom(j, k);
        if ((j - k) & 1) coeff = -coeff;       /* (-1)^(j-k) */
        seq_t term  = prime_power_cache[i + k];
        sum += coeff * term;
    }
    return sum;
}

/* ---------- build cache of prime powers (A000961) ----------------- */
static void init_prime_powers(void)
{
    if (prime_power_len) return;                  /* already built */

    const idx_t SIEVE_LIMIT = 20000;              /* enough to generate many primes */
    const idx_t MAX_VAL      = ((idx_t)1) << 31;   /* upper bound for prime powers */

    /* sieve */
    bool *is_prime = (bool*)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) is_prime[i] = true;
    for (idx_t p = 2; p * p <= SIEVE_LIMIT; ++p) {
        if (is_prime[p]) {
            for (idx_t q = p * p; q <= SIEVE_LIMIT; q += p)
                is_prime[q] = false;
        }
    }

    /* temporary buffer for collected prime powers */
    size_t buf_cap = 1024;
    seq_t *buf = (seq_t*)malloc(buf_cap * sizeof(seq_t));
    size_t buf_len = 0;

    /* include 1 */
    if (buf_len == buf_cap) {
        buf_cap <<= 1;
        buf = (seq_t*)realloc(buf, buf_cap * sizeof(seq_t));
    }
    buf[buf_len++] = (seq_t)1;

    /* for each prime, generate its powers */
    for (idx_t p = 2; p <= SIEVE_LIMIT; ++p) {
        if (!is_prime[p]) continue;
        uint64_t val = p;
        while (true) {
            if (buf_len == buf_cap) {
                buf_cap <<= 1;
                buf = (seq_t*)realloc(buf, buf_cap * sizeof(seq_t));
            }
            buf[buf_len++] = (seq_t)val;
            if (val > MAX_VAL / p) break;         /* avoid overflow */
            val *= p;
        }
        if (buf_len >= PRIME_POWER_CACHE_CAP) break;
    }
    free(is_prime);

    /* sort the values (simple insertion sort) */
    for (size_t i = 1; i < buf_len; ++i) {
        seq_t key = buf[i];
        size_t j = i;
        while (j > 0 && key < buf[j - 1]) {
            buf[j] = buf[j - 1];
            --j;
        }
        buf[j] = key;
    }

    /* deduplicate and copy to the permanent cache */
    size_t out_len = 0;
    for (size_t i = 0; i < buf_len && out_len < PRIME_POWER_CACHE_CAP; ++i) {
        if (out_len == 0 || buf[i] != buf[out_len - 1]) {
            prime_power_cache[out_len++] = buf[i];
        }
    }
    free(buf);
    prime_power_len = out_len;
}

/* ---------- public function -------------------------------------- */
seq_t a(idx_t n)
{
    /* ensure cache is ready */
    init_prime_powers();

    /* locate (i,j) on the antidiagonal of n */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) ++d;        /* d = i + j  */
    idx_t tri = d * (d + 1) / 2;
    idx_t t   = n - tri;                           /* t = j */
    idx_t i   = d - t;
    idx_t j   = t;

    /* safety: clamp if cache is unexpectedly small */
    if ((size_t)(i + j) >= prime_power_len) return 0;

    return A_ij(i, j);
}

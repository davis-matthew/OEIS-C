/*
 * A372726  Legendre's triangle read by rows. T(n, k) = L(n / prime(k)) where
 *          L(n/p) is the Legendre symbol, for n >= 0 and 2 <= k <= n + 2.
 * https://oeis.org/A372726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = Legendre symbol (row / prime(col)), triangle read by rows
 *            with rows indexed from 0. The row index is determined by the
 *            cumulative row lengths r(r+1)/2
 * Range:     i8 n <= 9617, i16 n <= 9617, i32 n <= 9617, i64 n <= 9617,
 *            i128 n <= 9617, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- required headers ---------- */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- static prime cache ---------- */
static uint32_t *prime_list = NULL;   /* primes[0] = 2, primes[1] = 3, ... */
static size_t   prime_cnt  = 0;       /* number of primes stored */
static size_t   prime_cap  = 0;       /* current allocated capacity */

/* ensure prime cache can hold at least 'need' primes */
static void ensure_prime_cap(size_t need)
{
    if (prime_cap >= need) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    uint32_t *new_list = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
    prime_list = new_list;
    prime_cap  = new_cap;
}

/* add next prime to the cache */
static void add_next_prime(void)
{
    uint64_t cand = prime_cnt ? (uint64_t)prime_list[prime_cnt - 1] + 1ULL : 2ULL;
    for (;; ++cand)
    {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime)
        {
            ensure_prime_cap(prime_cnt + 1);
            prime_list[prime_cnt++] = (uint32_t)cand;
            return;
        }
    }
}

/* return the k-th prime, 1‑based */
static uint32_t get_prime(idx_t k)
{
    while (prime_cnt < k) add_next_prime();
    return prime_list[k - 1];
}

/* find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static idx_t find_row(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;
    /* double high until it exceeds n */
    while ((__uint128_t)high * (high + 1ULL) / 2ULL <= n)
        high <<= 1u;
    /* binary search */
    while (low + 1u < high)
    {
        idx_t mid = (low + high) >> 1;
        if ((__uint128_t)mid * (mid + 1ULL) / 2ULL <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* modular exponentiation: base^exp mod mod (mod is odd prime >2) */
static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1ULL % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1ULL) result = (uint64_t)((__uint128_t)result * base % mod);
        base = (uint64_t)((__uint128_t)base * base % mod);
        exp >>= 1ULL;
    }
    return result;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* determine row, column and compute Legendre symbol */
    idx_t r = find_row(n);                            /* row index (0‑based) */
    __uint128_t cum_before = (__uint128_t)r * (r + 1ULL) / 2ULL;
    idx_t  offset = (idx_t)(n - cum_before);          /* 0‑based within the row */
    idx_t  k = offset + 2;                            /* column index (2‑based) */

    uint32_t p = get_prime(k);                        /* prime(k) */

    /* Legendre symbol (r / p) */
    if (r % p == 0ULL) return (seq_t)0;

    uint64_t exp = ((uint64_t)p - 1ULL) >> 1U;         /* (p-1)/2 */
    uint64_t res = mod_pow_u64(r % p, exp, p);

    if (res == 1ULL) return (seq_t)1;
    if (res == p - 1ULL) return (seq_t)(-1);
    return (seq_t)0; /* should never occur for odd prime p */
}

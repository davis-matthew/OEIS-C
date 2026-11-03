/*
 * A306530  a(n) is the smallest prime q such that Kronecker(q, prime(n)) = 1.
 * https://oeis.org/A306530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime q such that Kronecker(q, prime(n)) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9474 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Utility headers */
#include <stddef.h>
#include "../utils/is_prime_u64.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

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

/* --------------------------------------------------------------------------
 * Prime cache – stores small primes (uint64_t).  The cache is expanded
 * automatically when larger primes are needed during the search.
 * -------------------------------------------------------------------------- */
static uint64_t *prime_cache = NULL;   /* array of primes, ascending order   */
static size_t   prime_cnt   = 0;       /* number of primes stored            */
static size_t   prime_cap   = 0;       /* allocated capacity                 */

/* Ensure capacity for at least 'new_cap' primes */
static void prime_ensure_capacity(size_t new_cap)
{
    if (prime_cap == 0) {
        prime_cap = (new_cap > 8) ? new_cap : 8;
        prime_cache = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_cache) abort();
    } else if (prime_cap < new_cap) {
        size_t new_cap2 = prime_cap * 2;
        if (new_cap2 < new_cap) new_cap2 = new_cap;
        uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap2 * sizeof(uint64_t));
        if (!tmp) abort();
        prime_cache = tmp;
        prime_cap   = new_cap2;
    }
}

/* Append a new prime to the cache (assumed to be > last stored) */
static void prime_add(uint64_t p)
{
    prime_ensure_capacity(prime_cnt + 1);
    prime_cache[prime_cnt++] = p;
}

/* Generate the next prime after 'last'; skips even numbers */
static uint64_t prime_next(uint64_t last)
{
    uint64_t cand = last + 1;
    if (cand % 2ULL == 0ULL && cand != 2ULL) cand++; /* skip even */
    for (;;) {
        if (cand == 2ULL) return 2ULL;
        if (is_prime_big((seq_t)cand)) return cand;
        cand += 2ULL; /* only odd numbers after 2 */
    }
}

/* --------------------------------------------------------------------------
 * Main sequence function
 * -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialize prime cache with the first prime 2 */
    if (prime_cnt == 0) prime_add(2);

    /* Fetch the n-th prime (1‑based).  p fits in uint32_t for n ≤ 10000. */
    uint32_t p32 = get_prime(n);
    uint64_t  p   = (uint64_t)p32;   /* promote to 64‑bit for uniformity */

    /* Special handling for p = 2: Kronecker(q,2)=1 ⇔ q ≡ ±1 (mod 8). */
    if (p == 2ULL) {
        for (size_t idx = 0;; ++idx) {
            if (idx >= prime_cnt) {
                uint64_t last = prime_cache[prime_cnt - 1];
                prime_add(prime_next(last));
            }
            uint64_t q = prime_cache[idx];
            if (q == 2ULL) continue; /* skip q = 2, which is even */
            if ((q & 7ULL) == 1ULL || (q & 7ULL) == 7ULL) {
                return (seq_t)q;
            }
        }
    }

    /* For odd prime p, we look for the smallest prime q with Legendre symbol (q|p)=1. */
    for (size_t idx = 0;; ++idx) {
        if (idx >= prime_cnt) {
            uint64_t last = prime_cache[prime_cnt - 1];
            prime_add(prime_next(last));
        }
        uint64_t q = prime_cache[idx];
        /* (q|p) = 0 when q = p; skip those. */
        if (q == p) continue;
        uint64_t rem = q % p;              /* reduce modulo p – fits in 64‑bit */
        if (jacobi_symbol(rem, p) == 1) {
            return (seq_t)q;
        }
    }
}

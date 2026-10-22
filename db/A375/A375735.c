/*
 * A375735  First differences of non-prime-powers (inclusive).
 * https://oeis.org/A375735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* First differences of non‑prime‑powers (inclusive). */
/* a(n) = N(n+1) – N(n) where N(k) is the k‑th non‑prime‑power (1 is a
 * prime‑power but not a non‑prime‑power). */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* ---------- local storage ------------------------------------- */

static size_t seq_cap   = 0;  /* capacity of seq_cache            */
static size_t seq_sz    = 0;  /* number of stored non‑prime‑powers*/
static seq_t *seq_cache = NULL;/* array of non‑prime‑powers (0‑based) */

static size_t primes_cap   = 0;   /* capacity of primes array      */
static size_t primes_sz    = 0;   /* number of primes stored       */
static uint64_t *primes    = NULL;/* list of primes                */
static uint64_t prime_next = 3;    /* next integer to test for prime*/

/* ---------- helper: realloc with cast (C++ compatible) ---------- */

static void grow_uint64(void **ptr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
    if (new_cap < needed) new_cap = needed;
    *ptr = realloc(*ptr, new_cap * sizeof(uint64_t));
    *cap = new_cap;
}

static void grow_seq(void **ptr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    if (new_cap < needed) new_cap = needed;
    *ptr = realloc(*ptr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* ---------- integer square root (floor) ----------------------- */

static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t low = 0, high = n;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* ---------- ensure primes up to 'limit' ----------------------- */

static void ensure_prime_upto(uint64_t limit)
{
    if (limit < 2) return;

    if (primes_sz == 0) {            /* initialisation */
        grow_uint64((void **)&primes, &primes_cap, 1);
        primes[0] = 2;
        primes_sz = 1;
        prime_next = 3;
    }

    while (prime_next <= limit) {
        bool is_p = true;
        uint64_t sqrt_p = isqrt_uint64(prime_next);
        for (size_t i = 0; i < primes_sz && primes[i] <= sqrt_p; ++i) {
            if (prime_next % primes[i] == 0) {
                is_p = false;
                break;
            }
        }
        if (is_p) {
            grow_uint64((void **)&primes, &primes_cap, primes_sz + 1);
            primes[primes_sz++] = prime_next;
        }
        ++prime_next;
    }
}

/* ---------- test whether m is a prime power (including 1 & primes) ---------- */

static bool is_prime_power(uint64_t m)
{
    if (m < 2) return true;          /* 1 is treated as a prime‑power */

    uint64_t sqrt_m = isqrt_uint64(m);
    ensure_prime_upto(sqrt_m);

    for (size_t i = 0; i < primes_sz && primes[i] <= sqrt_m; ++i) {
        uint64_t p = primes[i];
        if (m % p == 0) {
            uint64_t r = m;
            while (r % p == 0) r /= p;
            return r == 1;           /* true only if all factors are p */
        }
    }
    /* if no divisor found, m is prime */
    return true;
}

/* ---------- make sure seq_cache contains at least n+1 entries -------- */

static void ensure_nonprimes(idx_t n)
{
    static uint64_t candidate = 2;     /* next integer to test */
    /* we need indices 0…n inclusive → size must be > n */
    while (seq_sz <= (size_t)n) {
        for (;;) {
            uint64_t m = candidate++;
            if (!is_prime_power(m)) {
                grow_seq((void **)&seq_cache, &seq_cap, seq_sz + 1);
                seq_cache[seq_sz++] = (seq_t)m;
                break;
            }
        }
    }
}

/* ---------- required function ------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* undefined below offset */
    ensure_nonprimes(n);
    return seq_cache[n] - seq_cache[n - 1];
}

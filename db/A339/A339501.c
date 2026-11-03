/*
 * A339501  a(n) = smallest prime number between prime(n) and 2*prime(n) such
 *          that the arithmetic progression (prime(n), a(n), ...) and which
 *          contains only primes has the maximum length.
 * https://oeis.org/A339501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime q in (prime(n), 2·prime(n)) such that the
 *            arithmetic progression (prime(n), q, ...) contains only primes
 *            and has maximal possible length
 * Range:     u8 n <= 36, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
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

/*  A fixed sieve upper bound sufficient for n up to 10000;
    the 10000th prime is 104729, so 2·prime < 210000. */
static const idx_t SIEVE_LIMIT = 10000000ULL;

/* Global data used by the sequence implementation. */
static bool           initialized = false;
static uint64_t      *all_primes   = NULL;   /* array of primes up to SIEVE_LIMIT */
static size_t           all_cnt      = 0;    /* number of primes in all_primes */
static uint8_t        *prime_bits   = NULL;   /* bitset: prime_bits[p] == 1 ⇔ p prime */

/* Initialise the global sieve and prime array once. */
static void init_global(void)
{
    if (initialized)
        return;

    /* Build the list of primes up to SIEVE_LIMIT. */
    build_primes_upto(SIEVE_LIMIT, &all_primes, &all_cnt);

    /* Allocate a simple byte‑array to test primality fast. */
    prime_bits = (uint8_t *)malloc(SIEVE_LIMIT + 1);
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i)
        prime_bits[i] = 0;
    for (size_t i = 0; i < all_cnt; ++i)
        prime_bits[all_primes[i]] = 1;

    initialized = true;
}

/* Fast primality test for numbers up to SIEVE_LIMIT, otherwise delegates
   to a big‑int primality routine. */
static inline bool is_prime_fast(uint64_t x)
{
    if (x <= SIEVE_LIMIT)
        return prime_bits[x] != 0;
    return is_prime_big((seq_t)x);
}

/* Sequence function.  It works for any build that supplies seq_t and idx_t.
   Calls are independent; results are cached via the global static data. */
seq_t a(idx_t n)
{
    init_global();

    /* 1.  The base prime p = prime(n).  */
    uint64_t p = get_prime(n);
    /* 2.  Limit for candidate q:  p < q < 2p  */
    uint64_t limit2p = 2 * p;

    /* Locate the interval of indices in all_primes that fall in (p, 2p). */
    size_t start, end;

    /* Find first prime > p. */
    size_t lo = 0, hi = all_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (all_primes[mid] <= p)
            lo = mid + 1;
        else
            hi = mid;
    }
    start = lo;

    /* Find first prime ≥ 2p. */
    lo = 0; hi = all_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (all_primes[mid] < limit2p)
            lo = mid + 1;
        else
            hi = mid;
    }
    end = lo;             /* end is exclusive */

    /* 3.  Evaluate each candidate q.  */
    int      best_len = 0;
    uint64_t best_q   = 0;

    for (size_t idx = start; idx < end; ++idx) {
        uint64_t q = all_primes[idx];
        uint64_t d = q - p;   /* common difference */

        int len = 2;          /* p and q are already prime */
        uint64_t next = p + 2 * d;

        while (is_prime_fast(next)) {
            ++len;
            next += d;
        }

        if (len > best_len || (len == best_len && q < best_q)) {
            best_len = len;
            best_q   = q;
        }
    }

    return (seq_t)best_q;
}

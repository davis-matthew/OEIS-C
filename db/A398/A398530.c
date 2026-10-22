/*
 * A398530  a(n) = Sum_{k=1..n} f(floor(n/k)), where f(n) = A034444(n) is the
 *          number of unitary divisors (or squarefree divisors) of n.
 * https://oeis.org/A398530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..n} 2^{ω(⌊n/k⌋)} where ω(m) is the number of
 *            distinct prime factors of m
 * Range:     u8 n <= 149, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* ------------------------------------------------------------------- */
/*   Global caches for sieve and for f(m) = 2^{ω(m)}                    */
/* ------------------------------------------------------------------- */
static idx_t   *spf_cache = NULL;   /* smallest prime factor, [0..cap-1] */
static size_t  spf_cap   = 0;        /* allocated size of spf_cache */

static seq_t   *f_cache   = NULL;   /* f(n) value, [0..cap-1] */
static size_t  f_cap     = 0;        /* allocated size of f_cache */

static idx_t   *prime_list= NULL;   /* list of primes found so far */
static size_t  prime_cnt = 0;       /* number of primes stored     */
static size_t  prime_cap = 0;       /* allocated size of prime_list */

/* -------------------------------------------------------------------
 * Helper: Allocate or grow an array with explicit cast to the
 * required pointer type.  The cast is necessary for C++ compilation.
 * ------------------------------------------------------------------- */
#define RESIZE_ARRAY(ptr, new_cap, type)                   \
    do {                                                  \
        type *tmp = (type *)realloc((ptr), new_cap * sizeof(type)); \
        if (!tmp) exit(1);                                \
        (ptr) = tmp;                                      \
    } while (0)

/* -------------------------------------------------------------------
 * Build or extend the sieve of Eratosthenes (linear sieve).
 * Also compute f_cache[i] = 2^{ω(i)} for all i up to `up_to`.
 * ------------------------------------------------------------------- */
static void ensure_caches(idx_t up_to)
{
    /* Grow spf_cache if necessary */
    if (spf_cap <= (size_t)up_to) {
        size_t new_cap = (size_t)up_to + 1;
        RESIZE_ARRAY(spf_cache, new_cap, idx_t);
        /* Initialise new entries to 0 */
        for (size_t i = spf_cap; i <= up_to; ++i) spf_cache[i] = 0;
        spf_cap = new_cap;
    }

    /* Grow f_cache if necessary */
    if (f_cap <= (size_t)up_to) {
        size_t new_cap = (size_t)up_to + 1;
        RESIZE_ARRAY(f_cache, new_cap, seq_t);
        /* New entries will be filled below */
        f_cap = new_cap;
    }

    /* Grow prime_list if likely to be needed */
    if (prime_cap <= (up_to / 10 + 10)) {
        size_t new_cap = (prime_cap > 0) ? prime_cap * 2 : 64;
        RESIZE_ARRAY(prime_list, new_cap, idx_t);
        prime_cap = new_cap;
    }

    /* ---- Linear sieve up to up_to -------------------------------- */
    for (idx_t i = 2; i <= up_to; ++i) {
        if (spf_cache[i] == 0) {               /* i is prime */
            spf_cache[i] = i;
            if (prime_cnt == prime_cap)
                RESIZE_ARRAY(prime_list, prime_cap * 2, idx_t);
            prime_list[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            idx_t p = prime_list[j];
            idx_t ip = i * p;
            if (ip > up_to || p > spf_cache[i]) break;
            spf_cache[ip] = p;
        }
    }

    /* ---- Compute f_cache[i] = 2^{ω(i)} --------------------------- */
    f_cache[0] = 0;
    f_cache[1] = 1;   /* ω(1) = 0 -> 2^0 = 1 */

    for (idx_t n = 2; n <= up_to; ++n) {
        idx_t x = n;
        idx_t last_p = 0;
        uint64_t omega = 0;
        while (x > 1) {
            idx_t p = spf_cache[x];
            if (p != last_p) {
                ++omega;
                last_p = p;
            }
            while (x % p == 0) x /= p;
        }
        seq_t val = (seq_t)1;
        for (uint64_t i = 0; i < omega; ++i) val <<= 1; /* multiply by 2 */
        f_cache[n] = val;
    }
}

/* -------------------------------------------------------------------
 * Main sequence function
 * ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_caches(n);

    seq_t result = (seq_t)0;
    for (idx_t k = 1; k <= n; ) {
        idx_t q = n / k;                 /* floor(n/k) */
        idx_t next = n / q + 1;          /* first k after this block */
        seq_t block_len = (seq_t)(next - k);
        result += f_cache[q] * block_len;
        k = next;
    }
    return result;
}

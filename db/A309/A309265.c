/*
 * A309265  Numbers k such that s + t = k with 0 < s < t where s and t-s are
 *          both prime.
 * https://oeis.org/A309265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309265: Numbers k such that s + t = k with 0 < s < t
   and both s and t - s are prime.
   Algorithm: iterate through integer k, checking whether there exists
   a prime p (0 < p < k/2) with k - 2*p prime.  Store all such k in a
   memoised array so that any call to a(idx_t n) returns a(n) efficiently. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/*------------------------------------------------------------*/
/* Prime sieve and lookup                                     */
/*------------------------------------------------------------*/
static uint64_t *prime_list = NULL;   /* dynamic array of primes       */
static size_t   prime_cnt   = 0;      /* number of stored primes       */
static bool     *prime_bool = NULL;   /* boolean sieve: prime_bool[x] */
static size_t   prime_limit = 0;      /* upper bound of sieve array    */

static void build_prime_sieve(size_t limit)
{
    /* Allocate and build a simple Sieve of Eratosthenes up to 'limit'. */
    prime_bool = (bool*)malloc((limit + 1) * sizeof(bool));
    for (size_t i = 0; i <= limit; ++i) prime_bool[i] = true;
    prime_bool[0] = prime_bool[1] = false;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (prime_bool[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                prime_bool[j] = false;
        }
    }

    /* Count primes so we can allocate exactly enough space. */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (prime_bool[i]) ++cnt;

    prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (prime_bool[i]) prime_list[idx++] = (uint64_t)i;

    prime_cnt   = cnt;
    prime_limit = limit;
}

/* Boolean primality test that uses the precomputed sieve, with a
   simple fallback trial division for numbers beyond the sieve range. */
static bool isPrimeFast(uint64_t n)
{
    if (n <= prime_limit) return prime_bool[(size_t)n];

    if (n < 4) return n > 1;

    for (size_t pi = 0; pi < prime_cnt && prime_list[pi] * prime_list[pi] <= n; ++pi)
        if (n % prime_list[pi] == 0) return false;
    return true;
}

/*------------------------------------------------------------*/
/* Sequence storage and memoisation                          */
/*------------------------------------------------------------*/
static seq_t *seq_buf = NULL;   /* dynamic array of generated terms */
static size_t seq_len = 0;      /* number of terms currently stored  */
static size_t seq_cap = 0;      /* allocated capacity                  */

static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_buf = (seq_t*)realloc(seq_buf, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/*------------------------------------------------------------*/
/* Public API: a(idx_t n)                                    */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; n=0 is outside the range */

    /* Initialise prime data lazily.  A safe upper bound is chosen
       so that 10,000 terms are found well before the sieve limit. */
    if (prime_list == NULL)
        build_prime_sieve(2000000);  /* 2 million is comfortably large */

    /* Persist the current k across calls.  Start checking from 6. */
    static idx_t current_k = 5;

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        ++current_k;                                 /* next candidate k */
        bool found = false;

        /* Check all primes p <= (k-2)/2; if k-2p is prime, accept k. */
        const uint64_t kk = (uint64_t)current_k;
        const uint64_t maxp = (kk - 2U) / 2U;

        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            uint64_t p = prime_list[pi];
            if (p > maxp) break;

            uint64_t q = kk - 2U * p;
            if (q >= 2U && isPrimeFast(q)) {
                found = true;
                break;
            }
        }

        if (found) {
            ensure_seq_cap(seq_len + 1);
            seq_buf[seq_len++] = (seq_t)current_k;
        }
    }

    return seq_buf[n - 1];
}

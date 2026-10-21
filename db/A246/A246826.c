/*
 * A246826  Numbers n such that there is no prime of a prime twin pair between
 *          n^2 + n and n^2 + 3*n + 2.
 * https://oeis.org/A246826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A246826
 * Sequence: Numbers n such that there is no prime of a prime twin pair
 *           between n^2 + n  and  n^2 + 3*n + 2
 * Function: seq_t a(idx_t n)
 *   a(1) = 0, a(2) = 10, a(3) = 26, a(4) = 30, a(5) = 36, ...
 * The implementation chooses n by brute‑force testing, caching all
 * discovered terms and extending a simple prime sieve on demand.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------- */
/* Global cache of discovered terms                                     */
/* ------------------------------------------------------------------- */
static seq_t *terms   = NULL;   /* a(1), a(2), … */
static size_t terms_cap = 0;    /* capacity of the array */
static size_t terms_cnt = 0;    /* number of terms stored */

/* ------------------------------------------------------------------- */
/* Simple sieve:  is_prime[i] == true  → composite or i<2               */
/* ------------------------------------------------------------------- */
static bool *is_prime = NULL;   /* array of size sieve_lim+1 */
static size_t sieve_lim = 0;    /* current sieve upper bound (inclusive) */
static uint64_t *primes = NULL; /* list of primes up to sieve_lim */
static size_t prime_cnt   = 0;  /* number of primes stored */

/* ------------------------------------------------------------------- */
/* Grow terms cache                                                     */
/* ------------------------------------------------------------------- */
static void grow_terms(size_t need)
{
    if (need <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;
    terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* ------------------------------------------------------------------- */
/* Rebuild sieve up to NEW_LIMIT (inclusive)                           */
/* ------------------------------------------------------------------- */
static void build_sieve(size_t new_limit)
{
    /* free old arrays */
    free(is_prime);
    free(primes);

    /* allocate new sieve array */
    is_prime = (bool*)calloc(new_limit + 1, sizeof(bool));
    sieve_lim = new_limit;

    /* Eratosthenes: mark composites */
    if (new_limit >= 0) {
        is_prime[0] = true;
        if (new_limit >= 1) is_prime[1] = true;
    }
    for (size_t i = 2; i * i <= new_limit; ++i) {
        if (!is_prime[i]) {
            for (size_t j = i * i; j <= new_limit; j += i)
                is_prime[j] = true;
        }
    }

    /* Count and store primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        if (!is_prime[i]) ++cnt;
    primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    prime_cnt = cnt;
    size_t idx = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        if (!is_prime[i]) primes[idx++] = i;
}

/* ------------------------------------------------------------------- */
/* Ensure sieve covers at least LIM                                     */
/* ------------------------------------------------------------------- */
static void ensure_sieve(size_t lim)
{
    if (sieve_lim >= lim) return;
    /* enlarge to at least lim; double the old limit to reduce rebuilds */
    size_t new_lim = lim;
    while (new_lim <= sieve_lim) new_lim <<= 1;
    build_sieve(new_lim);
}

/* ------------------------------------------------------------------- */
/* Return index of first prime >= v, or prime_cnt if none              */
/* ------------------------------------------------------------------- */
static size_t first_prime_ge(uint64_t v)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] >= v) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

/* ------------------------------------------------------------------- */
/* Check primality of a 64‑bit integer using the current sieve         */
/* ------------------------------------------------------------------- */
static inline bool is_prime_u64(uint64_t m)
{
    if (m > sieve_lim) return false; /* outside current sieve */
    return !is_prime[m];
}

/* ------------------------------------------------------------------- */
/* Generate terms up to index N (inclusive)                            */
/* ------------------------------------------------------------------- */
static void generate_terms(idx_t N)
{
    if (terms_cnt >= (size_t)N) return;          /* already enough */

    /* next candidate n to test */
    uint64_t next_n = (terms_cnt == 0) ? 0 : (uint64_t)terms[terms_cnt-1] + 1;

    while (terms_cnt < (size_t)N) {
        /* interval bounds: n^2 + n  …  n^2 + 3*n + 2 */
        unsigned __int128 sq = (unsigned __int128)next_n * next_n;
        uint64_t low  = (uint64_t)(sq + next_n);            /* n^2 + n   */
        uint64_t high = (uint64_t)(sq + 3*next_n + 2ULL);   /* n^2 + 3n + 2 */

        /* need primes up to high+2 for twin checks */
        ensure_sieve((size_t)high + 2);

        /* scan primes in [low, high] looking for a twin pair member */
        bool has_twin = false;
        size_t idx = first_prime_ge(low);
        for (; idx < prime_cnt && primes[idx] <= high; ++idx) {
            uint64_t p = primes[idx];
            if (p >= 3 && is_prime_u64(p - 2)) { has_twin = true; break; }
            if (is_prime_u64(p + 2)) { has_twin = true; break; }
        }

        if (!has_twin) {
            /* next_n qualifies as a term of the sequence */
            grow_terms(terms_cnt + 1);
            terms[terms_cnt++] = (seq_t)next_n;
        }
        ++next_n;
    }
}

/* ------------------------------------------------------------------- */
/* Main sequence function                                              */
/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined; avoid underflow */
    generate_terms(n);
    return terms[n-1];
}

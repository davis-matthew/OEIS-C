/*
 * A241912  Fixed points of A241916.
 * https://oeis.org/A241912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 4931, u32 n <= 4931, u64 n <= 4931,
 *            u128 n <= 4931, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A241912: fixed points of A241916.
   a(n) = the nth number such that
   - either n is a power of 2, or
   - if n = 2^v2 ⋅ p1^e1 ⋯ pk^ek (p1<p2<…<pk),
     then ek = v2 + 1, and the exponents e1 … e_{k-1}
     (including zeros for missing primes) form a palindrome.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------- */
/* Small‑prime sieve (linear sieve) – static data structures    */
/* ------------------------------------------------------------- */
static size_t   PRIMES_LIMIT = 5000000;          /* adjust high enough */
static uint32_t *spf = NULL;                    /* smallest prime factor */
static uint32_t *primes = NULL;                 /* list of primes   */
static size_t   primes_count = 0;

/* linear sieve up to PRIMES_LIMIT, allocates spf and primes     */
static void init_sieve(void)
{
    if (spf != NULL) return; /* already init */

    spf = (uint32_t*)malloc((PRIMES_LIMIT + 1) * sizeof(uint32_t));
    if (!spf) exit(EXIT_FAILURE);  /* allocation failure */

    primes = (uint32_t*)malloc(PRIMES_LIMIT * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);

    size_t cnt = 0;
    for (size_t i = 2; i <= PRIMES_LIMIT; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = (uint32_t)i;
            primes[cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < cnt &&
                           primes[j] <= spf[i] &&
                           i * (size_t)primes[j] <= PRIMES_LIMIT; ++j) {
            size_t v = i * primes[j];
            spf[v] = primes[j];
        }
    }
    primes_count = cnt;
}

/* binary search in the primes array (always sorted)             */
static inline size_t find_prime_index(uint32_t p)
{
    size_t lo = 0, hi = primes_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == p) return mid;
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return primes_count; /* not found – should not happen for primes */
}

/* ------------------------------------------------------------- */
/* Check whether a 64‑bit unsigned integer n satisfies the rule */
/* ------------------------------------------------------------- */
static bool is_good_uint64(uint64_t n)
{
    /* Count exponent of 2 */
    int v2 = 0;
    while ((n & 1ULL) == 0) {
        ++v2;
        n >>= 1;
    }

    /* If odd part is 1 → power of 2 */
    if (n == 1ULL)
        return true;

    uint64_t odd = n;  /* now odd part */

    /* Factorise the odd part using the precomputed SPF array */
    uint32_t pfs[16];
    int     eps[16];
    int     npf = 0;

    while (odd > 1 && npf < 16) {
        uint32_t p = spf[odd];
        int e = 0;
        while (odd % p == 0) {
            odd /= p;
            ++e;
        }
        pfs[npf] = p;
        eps[npf++] = e;
    }
    if (odd > 1) {                /* remaining prime > limit – unlikely */
        if (npf >= 16) return false;
        pfs[npf] = (uint32_t)odd;
        eps[npf++] = 1;
    }

    /* Most significant prime exponent must be v2 + 1 */
    int exp_max   = eps[npf - 1];
    int largest_p = (int)pfs[npf - 1];
    if (exp_max != v2 + 1)
        return false;

    /* Palindrome condition for intermediate exponents. */
    size_t idx_pmax = find_prime_index(largest_p);
    size_t left  = 1;                 /* start after 2 (prime 2 at index 0) */
    size_t right = idx_pmax - 1;      /* before largest prime */

    while (left <= right) {
        uint32_t prime_left  = primes[left];
        uint32_t prime_right = primes[right];

        int exp_left  = 0;
        int exp_right = 0;
        for (int i = 0; i < npf; ++i) {
            if (pfs[i] == prime_left)  exp_left  = eps[i];
            if (pfs[i] == prime_right) exp_right = eps[i];
        }

        if (exp_left != exp_right)
            return false;
        ++left; --right;
    }
    return true;
}

/* ------------------------------------------------------------- */
/* Main sequence generator with lazy cache                      */
/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialise sieve on first use */
    static bool sieve_done = false;
    if (!sieve_done) {
        init_sieve();
        sieve_done = true;
    }

    /* Static cache of terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_candidate = 1;   /* next integer to test */

    /* Lazy allocation of cache */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        if (is_good_uint64(next_candidate)) {
            /* Grow cache if needed */
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}

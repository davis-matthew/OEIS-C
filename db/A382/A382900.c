/*
 * A382900  Composites whose prime factors are not all Mersenne primes.
 * https://oeis.org/A382900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * This function returns the n‑th composite integer whose prime
 * factors are NOT all Mersenne primes.  The implementation
 * builds the sequence on demand with a small cache and an SPF
 * (smallest‐prime‐factor) sieve.  The sieve is enlarged as needed
 * when a larger number has to be examined.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* -------------------------------------------------------------
 * Local static data
 * ------------------------------------------------------------- */
static seq_t *seq_cache = NULL;      /* cached sequence values   */
static size_t seq_len  = 0;          /* number already cached    */
static size_t seq_cap  = 0;          /* capacity of seq_cache    */

static uint32_t *spf = NULL;        /* SPF array (uint32_t suffices) */
static uint64_t spf_limit = 0;       /* Largest number whose SPF is stored */

/* Known Mersenne primes that fit in uint64_t */
static const uint64_t mersenne_primes[] = {
    3ULL, 7ULL, 31ULL, 127ULL, 8191ULL,
    131071ULL, 524287ULL, 2147483647ULL,
    2305843009213693951ULL
};
static const size_t n_mersenne = sizeof(mersenne_primes)/sizeof(mersenne_primes[0]);

/* -------------------------------------------------------------
 * Helper: rebuild SPF up to at least 'limit'.
 * ------------------------------------------------------------- */
static void rebuild_spf(uint64_t limit)
{
    if (limit <= spf_limit) return;      /* already covered */

    uint64_t new_limit = limit;

    uint32_t *new_spf = (uint32_t *)malloc((new_limit + 1) * sizeof(uint32_t));
    for (uint64_t i = 0; i <= new_limit; ++i) new_spf[i] = (uint32_t)i;

    for (uint64_t i = 2; i * i <= new_limit; ++i) {
        if (new_spf[i] == (uint32_t)i) {            /* prime */
            for (uint64_t j = i * i; j <= new_limit; j += i) {
                if (new_spf[j] == (uint32_t)j) new_spf[j] = (uint32_t)i;
            }
        }
    }

    free(spf);
    spf = new_spf;
    spf_limit = new_limit;
}

/* -------------------------------------------------------------
 * Helper: test if a prime is a Mersenne prime.
 * ------------------------------------------------------------- */
static bool is_mersenne_prime(uint64_t p)
{
    for (size_t i = 0; i < n_mersenne; ++i) {
        if (mersenne_primes[i] == p) return true;
    }
    return false;
}

/* -------------------------------------------------------------
 * Helper: test if x is composite and its prime factors are NOT
 * all Mersenne primes.
 * ------------------------------------------------------------- */
static bool should_include(uint64_t x)
{
    if (x < 4) return false;                     /* 1,2,3 are not composites */
    if (x <= spf_limit && spf[x] == x) return false;  /* prime */

    uint64_t tmp = x;
    bool all_mersenne = true;

    while (tmp > 1) {
        uint64_t p = spf[tmp];
        if (p == 0) p = tmp;                    /* safety, should not happen */

        if (!is_mersenne_prime(p)) {
            all_mersenne = false;
            break;
        }

        /* divide all occurrences of p */
        do { tmp /= p; } while (tmp % p == 0);
    }
    return !all_mersenne;
}

/* -------------------------------------------------------------
 * Extend the cache so that it contains at least 'n' terms.
 * ------------------------------------------------------------- */
static void extend_cache(idx_t n)
{
    static uint64_t current = 2;    /* next integer to test */

    while (seq_len < n) {
        /* enlarge SPF array if needed */
        if (current > spf_limit) {
            uint64_t new_limit = spf_limit * 2;
            if (new_limit < current) new_limit = current;
            if (new_limit < 1000ULL) new_limit = 1000ULL;
            rebuild_spf(new_limit);
        }

        if (should_include(current)) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap == 0 ? 128 : seq_cap * 2;
                seq_cache = (seq_t *)realloc(seq_cache,
                                             seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)current;
        }
        ++current;
    }
}

/* -------------------------------------------------------------
 * Public function implementing OEIS sequence A382900.
 * ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* offset is 1, return 0 for n=0 */
    extend_cache(n);
    return seq_cache[n - 1];
}

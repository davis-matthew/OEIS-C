/*
 * A248666  Greatest common divisor of the coefficients of the polynomial
 *          p(n,x) defined in Comments.
 * https://oeis.org/A248666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd of the coefficients of the polynomial p(n,x)
 *            (computed multiplicatively): a(n) = ∏_{p∣n} g(p), where g(2) = 2
 *            if v₂(n)=1 g(2) = 4 if v₂(n)≥2 g(p) = p if p is an odd prime
 *            satisfying Σ_{k=0}^{p-1} (p‑1)!/(p‑1‑k)! ≡ 0 (mod p) g(p) = 1
 *            otherwise
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of the types supplied by the build */
extern seq_t gcd(seq_t a, seq_t b);

/* ---------- prime sieve and caching ---------- */
static idx_t *primes = NULL;          /* array of primes */
static size_t primes_cnt = 0;         /* number of primes stored */
static idx_t sieve_limit = 0;         /* up to which number the sieve was built */

/* Simple incremental sieve – only called once */
static void init_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* Allocate temporary array for composites */
    bool *is_comp = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_comp)
        exit(1);

    for (idx_t i = 2; i * i <= limit; ++i)
        if (!is_comp[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;

    /* Build prime list */
    if (primes)
        free(primes);
    primes_cnt = 0;
    primes = (idx_t *)malloc((size_t)(limit / 10 + 10) * sizeof(idx_t));
    if (!primes)
        exit(1);

    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) {
            if (primes_cnt == (size_t)(limit / 10 + 10)) {
                size_t new_cap = (size_t)(limit / 10 + 10) * 2;
                idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
                if (!tmp)
                    exit(1);
                primes = tmp;
            }
            primes[primes_cnt++] = i;
        }

    free(is_comp);
    sieve_limit = limit;
}

/* ---------- "good prime" cache ---------- */
static unsigned char *good_cache = NULL;   /* 0 = unknown, 1 = good, 2 = bad */
static size_t good_cache_len = 0;          /* current length of array */

static bool is_good_prime(idx_t p)
{
    /* Ensure the cache can hold index p */
    if (p >= good_cache_len) {
        size_t new_len = p + 1;
        unsigned char *tmp = (unsigned char *)realloc(good_cache, new_len);
        if (!tmp)
            exit(1);
        /* Zero the newly added part */
        if (good_cache_len < new_len)
            for (size_t i = good_cache_len; i < new_len; ++i)
                tmp[i] = 0;
        good_cache = tmp;
        good_cache_len = new_len;
    }

    unsigned char v = good_cache[p];
    if (v != 0)
        return v == 1;

    if (p == 2) {            /* by convention 2 is good (handled separately) */
        good_cache[p] = 1;
        return true;
    }

    idx_t sum  = 0;
    idx_t term = 1;          /* term for k = 0 */

    for (idx_t k = 0; k < p; ++k) {
        if (k > 0)
            term = (term * (p - k)) % p;   /* multiply by next factor (p‑k) */
        sum = (sum + term) % p;
    }

    bool good = (sum == 0);
    good_cache[p] = good ? 1 : 2;
    return good;
}

/* ---------- sequence computation ---------- */
static bool seq_initialized = false;
static void init_sequence(void)
{
    if (seq_initialized)
        return;
    /* A sieve large enough for the test limits (≈10 000) */
    init_sieve(20000);
    seq_initialized = true;
}

/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;    /* a(1) = 1 (a(0) is undefined but harmless) */

    init_sequence();

    idx_t x = n;
    seq_t result = (seq_t)1;

    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= x; ++i) {
        idx_t p = primes[i];
        if (x % p == 0) {
            size_t exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            if (p == 2) {
                if (exp == 1)
                    result *= (seq_t)2;
                else if (exp >= 2)
                    result *= (seq_t)4;
            } else if (is_good_prime(p)) {
                result *= (seq_t)p;
            }
        }
    }

    if (x > 1) {          /* remaining prime factor > sqrt(original n) */
        idx_t p = x;
        if (p == 2)
            result *= (seq_t)2;
        else if (is_good_prime(p))
            result *= (seq_t)p;
    }

    return result;
}

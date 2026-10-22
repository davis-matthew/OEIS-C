/*
 * A340019  MM-numbers of labeled graphs with half-loops, without isolated
 *          vertices.
 * https://oeis.org/A340019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer that is squarefree and whose prime
 *            factors' indices among the primes are either prime or a
 *            square‑free semiprime. The number 1 is considered valid
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static const idx_t MAX_SIEVE = 2000000;          /* sieve limit sufficient for n ≤ 10000 */

static int  *primes      = NULL;                /* list of primes (for factoring) */
static size_t prime_count = 0;                  /* number of primes in list   */
static int  *prime_index = NULL;                /* prime number → its 1‑based index */
static bool *is_prime    = NULL;                /* primality up to MAX_SIEVE  */
static bool *allowed     = NULL;                /* indices allowed (prime or sf‑semiprime) */
static size_t cache_len   = 0;                  /* number of cached terms     */
static size_t cache_cap   = 0;                  /* cache capacity             */
static seq_t *cache       = NULL;                /* cached sequence values     */
static idx_t  next_candidate = 1;               /* next integer to test       */

/* ------------------------------------------------------------------------- */
/*  Prime sieve and auxiliary tables                                         */
/* ------------------------------------------------------------------------- */
static void init_sieve(void)
{
    if (primes) return;                       /* already initialized */

    /* allocate arrays */
    is_prime    = (bool *)malloc((MAX_SIEVE + 1) * sizeof(bool));
    prime_index = (int  *)calloc(MAX_SIEVE + 1, sizeof(int));
    if (!is_prime || !prime_index) exit(1);

    /* Sieve of Eratosthenes */
    for (idx_t i = 0; i <= MAX_SIEVE; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t i = 2; i * i <= MAX_SIEVE; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= MAX_SIEVE; j += i)
                is_prime[j] = false;
        }
    }

    /* build prime list and prime_index mapping */
    primes = (int *)malloc((MAX_SIEVE / 10 + 100) * sizeof(int));
    if (!primes) exit(1);

    for (idx_t i = 2; i <= MAX_SIEVE; ++i) {
        if (is_prime[i]) {
            primes[prime_count++] = (int)i;
            prime_index[i] = (int)prime_count;        /* 1‑based index */
        }
    }

    /* compute allowed array for indices 1..prime_count */
    allowed = (bool *)calloc(prime_count + 1, sizeof(bool));
    if (!allowed) exit(1);

    /* first, mark prime indices as allowed */
    for (int i = 2; i <= (int)prime_count; ++i) {
        if (is_prime[i]) allowed[i] = true;
    }

    /* build smallest‑prime‑factor array up to prime_count */
    int *spf = (int *)calloc(prime_count + 1, sizeof(int));
    if (!spf) exit(1);
    for (int i = 2; i <= (int)prime_count; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((long long)i * i <= prime_count) {
                for (int j = i * i; j <= (int)prime_count; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* mark square‑free semiprimes as allowed */
    for (int i = 2; i <= (int)prime_count; ++i) {
        int p1 = spf[i];
        if (p1 == 0) continue;
        int temp = i / p1;
        int p2 = spf[temp];
        if (p2 == 0) continue;
        if (p1 == p2) continue;               /* not square‑free */
        if ((long long)p1 * p2 == i)
            allowed[i] = true;                 /* semiprime */
    }

    free(spf);
}

/* ------------------------------------------------------------------------- */
/*  Test whether a number satisfies the required property                    */
/* ------------------------------------------------------------------------- */
static bool is_valid(idx_t val)
{
    if (val == 1) return true;          /* by definition */

    idx_t temp = val;
    for (size_t i = 0; i < prime_count && (idx_t)primes[i] * (idx_t)primes[i] <= temp; ++i) {
        int p = primes[i];
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
                if (exp > 1) return false;  /* square factor */
            }
            int idx = prime_index[p];
            if (idx == 0 || !allowed[idx]) return false;    /* invalid index */
        }
    }

    if (temp > 1) {                         /* remaining prime factor > sqrt(val) */
        int idx = prime_index[(int)temp];
        if (idx == 0 || !allowed[idx]) return false;
    }

    return true;
}

/* ------------------------------------------------------------------------- */
/*  Main function : a(n)                                                     */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) { init_sieve(); initialized = true; }

    while (cache_len < n) {
        idx_t val = next_candidate++;
        if (is_valid(val)) {
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) exit(1);
            }
            cache[cache_len++] = (seq_t)val;
        }
    }
    return cache[n - 1];
}

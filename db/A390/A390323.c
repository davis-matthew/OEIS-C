/*
 * A390323  Sum of composite numbers coprime to n in the range 1 < k < n.
 * https://oeis.org/A390323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{1<k<n, gcd(n,k)=1, k composite} k
 * Range:     u8 n <= 28, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The types idx_t (unsigned 64‑bit) and seq_t (unsigned integer of varying size) are
 * supplied by the build system; they are not defined here. */

#include <stdlib.h>
#include <stdbool.h>

/* Euclidean gcd for idx_t values */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Prime sieve state */
static bool *prime_flag = NULL;   /* prime_flag[i] == true iff i is prime */
static idx_t prime_max = 0;       /* current maximum n for which prime_flag is valid */

/* Ensure that prime_flag is accurate up to 'limit' (inclusive) */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_max) return;   /* already sufficient */

    /* Allocate or enlarge the flag array */
    bool *new_flag = (bool *)realloc(prime_flag, (size_t)(limit + 1) * sizeof(bool));
    if (!new_flag) abort();           /* allocation failed */
    prime_flag = new_flag;

    /* Initialize new entries as potential primes */
    for (idx_t i = prime_max + 1; i <= limit; ++i)
        prime_flag[i] = true;

    /* 0 and 1 are not prime */
    if (limit >= 0) prime_flag[0] = false;
    if (limit >= 1) prime_flag[1] = false;

    /* Sieve from existing limit+1 upward using primes already known */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (prime_flag[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                prime_flag[m] = false;
        }
    }

    prime_max = limit;
}

/* Cache for computed a(n) values */
static seq_t *cache_vals = NULL;   /* cache_vals[n] holds a(n) */
static idx_t cache_max = 0;        /* largest n for which cache_vals is valid */

/* Ensure that cache_vals contains values up to 'n' */
static void ensure_cache(idx_t n)
{
    if (cache_max >= n) return;      /* already have the needed values */

    /* Resize cache array */
    seq_t *new_cache = (seq_t *)realloc(cache_vals,
                                        (size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache) abort();
    cache_vals = new_cache;

    /* Ensure prime sieve up to n-1 for composite check */
    if (n >= 2) ensure_primes(n - 1);
    else ensure_primes(0);

    /* Compute new terms */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        if (i <= 2) {
            cache_vals[i] = 0;
            continue;
        }
        for (idx_t k = 2; k < i; ++k) {
            if (gcd_idx(i, k) == 1 && !prime_flag[k]) {
                sum += (seq_t)k;
            }
        }
        cache_vals[i] = sum;
    }

    cache_max = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* out‑of‑range, but harmless */
    ensure_cache(n);
    return cache_vals[n];
}

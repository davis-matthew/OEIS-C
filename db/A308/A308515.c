/*
 * A308515  Take all the integer-sided triangles with perimeter n and side
 *          lengths a, b and c such that a <= b <= c and c is prime. a(n) is
 *          the sum of all the b's.
 * https://oeis.org/A308515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over all primes c <= n-2 with n-c > c of sum_{a =
 *            max(1, n-c-c)}^{⌊(n-c)/2⌋} (n - c - a), where a ≤ b = n - c - a
 *            ≤ c and a + b > c
 * Range:     u8 n <= 38, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: sum of all the middle side lengths b of integer-sided triangles
   with perimeter n, side lengths a ≤ b ≤ c, and c prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- Prime cache ---------- */
static idx_t prime_cache_limit = 0;   /* largest number for which primes are cached */
static uint64_t *prime_cache = NULL;  /* array of primes */
static size_t prime_count = 0;        /* number of cached primes */

/* Ensure primes up to `limit` are cached. */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_cache_limit) return;

    /* Allocate boolean array for sieve */
    size_t sz = (size_t)(limit + 1);
    char *is_prime = (char *)calloc(sz, sizeof(char));
    if (!is_prime) return;            /* OOM – caller will receive 0 result */

    /* Mark all numbers ≥2 as prime initially */
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = 1;

    /* Sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!is_prime[p]) continue;
        for (idx_t m = p * p; m <= limit; m += p)
            is_prime[m] = 0;
    }

    /* Count primes to allocate the array */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    uint64_t *pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) pr[pos++] = (uint64_t)i;

    free(is_prime);

    /* Replace old cache */
    free(prime_cache);
    prime_cache = pr;
    prime_count  = cnt;
    prime_cache_limit = limit;
}

/* ------------------- Main sequence function ------------------- */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n < 5) return 0;           /* trivial zeros for n = 1..4 */

    ensure_primes(n);              /* prime list needed up to n */

    for (size_t pi = 0; pi < prime_count; ++pi) {
        idx_t c = (idx_t)prime_cache[pi];
        if (c > n - 2) break;      /* largest side cannot exceed n-2 */

        idx_t S = n - c;           /* remaining perimeter a + b */
        if (S <= c) continue;      /* triangle inequality a + b > c fails */

        idx_t lower_a = S - c;     /* by b ≤ c  => a ≥ S - c */
        if (lower_a < 1) lower_a = 1;

        idx_t upper_a = S / 2;     /* by a ≤ b  => a ≤ S/2 */

        for (idx_t a_val = lower_a; a_val <= upper_a; ++a_val) {
            idx_t b_val = S - a_val;   /* b = S - a */
            sum += (seq_t)b_val;
        }
    }

    return sum;
}

/*
 * A308257  Take all the integer-sided triangles with perimeter n and nonprime
 *          sides a, b, and c such that a <= b <= c. a(n) is the sum of all
 *          the b's.
 * https://oeis.org/A308257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force enumeration of all integer‑sided
   triangles with perimeter n.  For each triple a ≤ b ≤ c
   such that a, b and c are not prime (1 is allowed) and a + b > c,
   the middle side b is added to the sum. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static prime sieve cache up to the largest n requested. */
static bool *prime_cache = NULL;
static idx_t prime_cache_size = 0;

/* Ensure the prime cache contains primality information up to N. */
static void init_prime_cache(idx_t N)
{
    if (N <= prime_cache_size)
        return;

    /* Allocate a new cache of size N+1, initialized to false. */
    bool *new_cache = (bool *)calloc((size_t)(N + 1), sizeof(bool));
    if (!new_cache) {
        /* If allocation fails, the program cannot continue. */
        free(prime_cache);
        prime_cache = NULL;
        prime_cache_size = 0;
        return;
    }

    free(prime_cache);
    prime_cache = new_cache;
    prime_cache_size = N;

    if (N < 2)
        return;

    /* Mark all integers ≥ 2 as potential primes. */
    for (idx_t i = 2; i <= N; ++i)
        prime_cache[i] = true;

    /* Sieve of Eratosthenes. */
    for (idx_t i = 2; (idx_t)i * i <= N; ++i) {
        if (prime_cache[i]) {
            for (idx_t j = i * i; j <= N; j += i)
                prime_cache[j] = false;
        }
    }
}

/* Sequence function: a(n) = sum of all middle sides b. */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;              /* No triangle can exist. */

    init_prime_cache(n);       /* Ensure primality data up to n. */

    seq_t sum = 0;

    /* Iterate over the smallest side a (1 ≤ a ≤ n/3). */
    idx_t a_max = n / 3;
    for (idx_t a_val = 1; a_val <= a_max; ++a_val) {
        if (prime_cache[a_val])
            continue;          /* a must be non‑prime. */

        /* Compute bounds for the middle side b. */
        idx_t b_start = a_val;
        idx_t b_end   = (n - a_val) / 2;   /* ensures b ≤ c. */

        /* Triangle inequality: a + b > c -> 2(a + b) > n. */
        idx_t b_min_triangle = (n / 2) - a_val;
        if ((n & 1) && ((n / 2) - a_val == (n / 2)) && ((n / 2) - a_val < a_val))
            /* For odd n, need strict >; adjust accordingly. */
            b_min_triangle += 1;
        if (b_min_triangle < a_val)
            b_min_triangle = a_val;

        if (b_start < b_min_triangle)
            b_start = b_min_triangle;

        for (idx_t b_val = b_start; b_val <= b_end; ++b_val) {
            if (prime_cache[b_val])
                continue;      /* b must be non‑prime. */

            idx_t c_val = n - a_val - b_val;
            if (c_val < b_val)
                continue;      /* maintain ordering a ≤ b ≤ c. */

            if (prime_cache[c_val])
                continue;      /* c must be non‑prime. */

            if (a_val + b_val <= c_val)
                continue;      /* triangle inequality violated. */

            sum += (seq_t)b_val;  /* Add middle side to the sum. */
        }
    }

    return sum;
}

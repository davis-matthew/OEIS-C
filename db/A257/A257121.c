/*
 * A257121  Numbers m with 9*m + 3*r - 1 and 9*m + 3*r + 1 twin prime for some
 *          r = 0,1,2.
 * https://oeis.org/A257121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer m such that there exists r∈{0,1,2} with
 *            9*m+3*r−1 and 9*m+3*r+1 both prime
 * Range:     u8 n <= 67, u16 n <= 5219, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS utilities are not needed for the primality test, we
   provide a simple deterministic test for uint64_t numbers. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Simple deterministic primality test for unsigned 64‑bit integers.
   Since 9*m+3*r±1 stays far below 2^63 for the required range, 3‑loop
   trial division is more than fast enough. */
static bool is_prime_u64(uint64_t x)
{
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if ((x & 1) == 0) return false;
    for (uint64_t i = 3; i * i <= x; i += 2)
        if (x % i == 0) return false;
    return true;
}

/* Sequence accessor with memoisation. */
seq_t a(idx_t n)
{
    /* Storage for already computed terms.  The cache is
       dynamically grown as needed. */
    static seq_t *cache      = NULL;   /* array of found m          */
    static size_t cache_cap = 0;       /* capacity of the cache    */
    static size_t cache_len = 0;       /* number of stored terms    */

    /* We keep the next candidate as a 64‑bit integer so we do
       not overflow when the underlying seq_t type is too small. */
    static uint64_t next_candidate = 0;

    if (n == 0)          /* the sequence is 1‑based; ignore 0 */
        return 0;

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        uint64_t m = next_candidate;
        bool ok = false;

        for (int r = 0; r <= 2 && !ok; ++r) {
            uint64_t p = 9ULL * m + 3ULL * (uint64_t)r - 1ULL;
            uint64_t q = 9ULL * m + 3ULL * (uint64_t)r + 1ULL;

            /* The primes 2 and 3 are already handled by the primality
               test; we only need to check that p and q are ≥ 2. */
            if (p >= 2 && q >= 2 &&
                is_prime_u64(p) && is_prime_u64(q))
                ok = true;
        }

        if (ok) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* unrecoverable allocation error */
                cache      = tmp;
                cache_cap  = new_cap;
            }
            /* Store the found m, truncating to seq_t if necessary.
               For build types where seq_t is larger than 64 bits this
               is harmless; for smaller types the values beyond the
               representable range are not required by the tests. */
            cache[cache_len++] = (seq_t)m;
        }

        ++next_candidate;
    }

    return cache[n - 1];
}

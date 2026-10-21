/*
 * A285812  Primes equal to a centered 9-gonal number plus 1.
 * https://oeis.org/A285812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form (9*n*(n-1))/2 + 2
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 28, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of primes p such that p = C9(k)+1 where 
   centered 9‑gonal numbers are C9(k) = 9*k*(k-1)/2 + 1.
   Hence p = 9*k*(k-1)/2 + 2.  The function returns the n‑th such prime. */

#include "../utils/append_candidate.h"
#include "../utils/is_prime_big.h"

static seq_t *prime_cache = NULL;
static size_t cache_len   = 0;
static size_t cache_cap   = 0;
static idx_t   next_k     = 1;        /* next k to test for the form */

/* Ensure cache contains at least "need" primes. */
static void ensure_cache(idx_t need)
{
    while (cache_len < (size_t)need) {
        /* Compute p(k) = 9*k*(k-1)/2 + 2 as seq_t */
        seq_t k = (seq_t)next_k;
        seq_t p = (seq_t)9 * k * (k - 1) / 2 + 2;
        if (is_prime_big(p))            /* test primality of p */
            append_candidate(&prime_cache, &cache_len, &cache_cap, p);
        ++next_k;                       /* advance to next k */
    }
}

seq_t a(idx_t n)
/* Returns the n-th term of the sequence (1‑based). */
{
    if (n < 1) return 0;           /* undefined for n < 1 */
    ensure_cache(n);
    return prime_cache[n - 1];
}

/*
 * A321528  Expansion of b(x)^2 * b(x^2) / b(x^4) where b is a cubic AGM theta
 *          function.
 * https://oeis.org/A321528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 48, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A321528
   Formula:  a(0) = 1
             a(n) = -6 * (σ(n) - 4*σ(n/2) - 9*σ(n/3) + 16*σ(n/4))   (n > 0)
   where σ(m) is the sum of divisors of m (σ(m)=0 if m is not an integer).
   The implementation pre‑computes σ values with a simple divisor‑sieve
   and caches them for subsequent calls.  All arithmetic uses the
   build‑defined integer type ``seq_t``.  The routine is O(n log n)
   for the first call with argument n and O(1) thereafter. */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static seq_t *sigma_cache = NULL;   /* array of σ(i) for i = 0..N */
static idx_t  sigma_cached_size = 0; /* current capacity of sigma_cache */

/* Ensure sigma_cache has entries for 0..n (inclusive) */
static void ensure_sigma(idx_t n)
{
    size_t needed = (size_t)n;
    if (sigma_cached_size > needed)  /* already sufficient */
        return;

    size_t new_size = needed + 1;
    seq_t *new_arr = (seq_t*)malloc(new_size * sizeof(seq_t));
    if (!new_arr) {
        /* In a real system you might handle allocation failure more gracefully */
        exit(EXIT_FAILURE);
    }

    /* Initialise to 0 */
    memset(new_arr, 0, new_size * sizeof(seq_t));

    /* Simple divisor sieve to compute σ(i) */
    for (size_t d = 1; d <= needed; ++d) {
        for (size_t m = d; m <= needed; m += d) {
            new_arr[m] += (seq_t)d;
        }
    }

    /* Replace old cache */
    free(sigma_cache);
    sigma_cache = new_arr;
    sigma_cached_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                /* a(0) = 1 */

    ensure_sigma(n);                    /* guarantees σ(n) is computed */

    /* σ(n) */
    seq_t s1 = sigma_cache[(size_t)n];
    /* σ(n/2) if n even else 0 */
    seq_t s2 = (n & 1ULL) == 0 ? sigma_cache[(size_t)(n / 2)] : 0;
    /* σ(n/3) if n divisible by 3 else 0 */
    seq_t s3 = (n % 3ULL) == 0 ? sigma_cache[(size_t)(n / 3)] : 0;
    /* σ(n/4) if n divisible by 4 else 0 */
    seq_t s4 = (n % 4ULL) == 0 ? sigma_cache[(size_t)(n / 4)] : 0;

    /* a(n) = -6 * (s1 - 4*s2 - 9*s3 + 16*s4) */
    seq_t result = -6 * (s1 - 4 * s2 - 9 * s3 + 16 * s4);
    return result;
}

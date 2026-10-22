/*
 * A360006  Least positive integer whose prime indices have median n/2. a(1) =
 *          1.
 * https://oeis.org/A360006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = 1                 (n = 1)
    a(n) = prime_{n/2}       (n even, n ≥ 2)
    a(n) = 2 * prime_{n-1}   (n odd, n ≥ 3)   */

/*  The function lazily generates primes with a simple incremental
    sieve: primes are stored in a dynamic array and new candidates
    are checked against the primes already generated.  The array is
    expanded as needed.  The algorithm works for any n ≥ 1 and
    satisfies the performance requirement for n up to about 10 000.  */

#include <stdlib.h>
#include <stdbool.h>

static uint64_t *prime_arr   = NULL;   /* primes 1‑based stored 0‑based */
static size_t    prime_cnt   = 0;      /* number of primes stored   */
static size_t    prime_cap   = 0;      /* current allocation size   */
static uint64_t  last_pr     = 1;      /* largest prime found so far */

/* Grow the prime array when needed. */
static void grow_scale_primes(void)
{
    if (prime_cap == 0) {
        prime_cap = 64;
        prime_arr = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    } else if (prime_cnt >= prime_cap) {
        prime_cap *= 2;
        prime_arr = (uint64_t *)realloc(prime_arr, prime_cap * sizeof(uint64_t));
    }
}

/* Test whether n is prime.  n is assumed to be > last_pr, so all primes
   needed for trial division are already in prime_arr. */
static bool is_prime_candidate(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_arr[i];
        if (p > n / p)   /* p*p > n but avoid overflow */
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* Generate primes until we have at least k primes (1‑based). */
static void ensure_primes(size_t k)
{
    if (prime_cnt >= k)
        return;

    while (prime_cnt < k) {
        grow_scale_primes();
        /* find next prime after last_pr */
        uint64_t candidate = last_pr + 1;
        for (;;) {
            if (is_prime_candidate(candidate)) {
                prime_arr[prime_cnt++] = candidate;
                last_pr = candidate;
                break;
            }
            ++candidate;
        }
    }
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n < 1, but return 0 gracefully */
        return (seq_t)0;

    if (n == 1)
        return (seq_t)1;

    if ((n & 1) == 0) {           /* even n */
        idx_t k = n >> 1;         /* n/2, 1‑based prime index */
        ensure_primes((size_t)k);
        return (seq_t)prime_arr[k - 1];
    } else {                      /* odd n ≥ 3 */
        idx_t k = n - 1;          /* prime_{n-1} */
        ensure_primes((size_t)k);
        return (seq_t)2 * (seq_t)prime_arr[k - 1];
    }
}

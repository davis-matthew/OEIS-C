/*
 * A374048  a(n) = 1 if A328846(n) is even, otherwise 0, where A328846 is the
 *          second Fibonacci based variant of arithmetic derivative.
 * https://oeis.org/A374048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the second Fibonacci‐based arithmetic derivative of
 *            n is even, 0 otherwise. The implementation uses the rules: for
 *            odd primes pₖ (0‑based index, 2 → index 0), the derivative
 *            follows the quadratic Leibniz rule with prime derivative Fₖ. The
 *            parity of the derivative is odd iff an odd num...
 * Range:     u8 n <= 9946, u16 n <= 9946, u32 n <= 9946, u64 n <= 9946,
 *            u128 n <= 9946, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* forward declarations of static helpers */
static void ensure_primes_upto(idx_t limit);
static idx_t prime_index_of(idx_t p);

/* static data for the prime list */
static idx_t *prime_list = NULL;
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* Ensure that prime_list contains all primes ≤ limit (0‑based indices). */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit) return;                 /* already enough */

    /* simple sieve up to limit */
    idx_t *is_prime = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!is_prime) return;                           /* out of memory – unlikely here */

    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    idx_t *new_list = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!new_list) { free(is_prime); return; }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) new_list[idx++] = i;

    free(is_prime);
    free(prime_list);              /* replace old list, if any */
    prime_list = new_list;
    prime_count = cnt;
    prime_limit = limit;
}

/* Binary search for 0‑based index of a prime p in prime_list. */
static idx_t prime_index_of(idx_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p) return (idx_t)mid;
        if (prime_list[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return (idx_t)-1;   /* not found – should not happen for valid primes */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Even numbers always give even derivatives. */
    if ((n & (idx_t)1) == 0) return 1;

    /* Ensure we have primes up to n for factorisation and indexing. */
    ensure_primes_upto(n);

    idx_t temp = n;
    int odd_contrib = 0; /* parity of contributing primes */

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p * p > temp) break;              /* remaining part is prime or 1 */

        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            if ((exp & 1) && (i % 3 != 0))
                odd_contrib ^= 1;
        }
    }

    if (temp > 1) {          /* remaining prime factor */
        idx_t p = temp;
        idx_t idx = prime_index_of(p);          /* 0‑based index of p */
        if ((idx % 3) != 0)                     /* exponent is 1 (odd) */
            odd_contrib ^= 1;
    }

    return odd_contrib ? 0 : 1;
}

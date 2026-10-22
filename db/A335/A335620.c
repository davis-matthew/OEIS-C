/*
 * A335620  The depth (number of inherent divisions plus 1 for the initial
 *          integer) of the continued fraction form of the rational number
 *          n/pi(n), where n is an integer >= 2 and pi(n) is the number of
 *          primes <= n.
 * https://oeis.org/A335620
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the depth of the continued‑fraction of n/pi(n)
   (number of Euclidean algorithm iterations plus one). */
#include <stdlib.h>

/*------- Prime caching (incremental sieve) -------*/
static idx_t prime_limit = 1;       /* largest n for which primes are known   */
static size_t prime_cap   = 0;       /* capacity of prime array                */
static size_t prime_len   = 0;       /* number of primes stored                */
static idx_t *prime_arr   = NULL;    /* array of primes                       */

/* Add a prime to prime_arr, resizing if necessary. */
static void add_prime(idx_t p)
{
    if (prime_len == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 64;
        idx_t *tmp = (idx_t *)realloc(prime_arr, new_cap * sizeof(idx_t));
        if (!tmp) abort();            /* abort on allocation failure          */
        prime_arr = tmp;
        prime_cap = new_cap;
    }
    prime_arr[prime_len++] = p;
}

/* Extend the prime cache up to max_n (inclusive).  Uses incremental
   primality testing with the existing prime list. */
static void extend_primes(idx_t max_n)
{
    if (max_n <= prime_limit) return;
    for (idx_t num = prime_limit + 1; num <= max_n; ++num) {
        int is_p = 1;
        for (size_t i = 0; i < prime_len; ++i) {
            idx_t p = prime_arr[i];
            if (p * p > num) break;
            if (num % p == 0) { is_p = 0; break; }
        }
        if (is_p) add_prime(num);
    }
    prime_limit = max_n;
}

/* Return the number of primes <= n using binary search on prime_arr. */
static size_t pi_of(idx_t n)
{
    if (n < 2) return 0;
    size_t lo = 0, hi = prime_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_arr[mid] <= n) lo = mid + 1;
        else hi = mid;
    }
    return lo; /* number of primes <= n */
}

/*------ Sequence function ------*/
seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* sequence defined only for n >= 2 */

    /* Ensure primes up to n are known. */
    extend_primes(n);

    /* Compute pi(n). */
    size_t pi_cnt = pi_of(n);       /* pi(n) fits in size_t */
    idx_t divisor = (idx_t)pi_cnt;  /* divisor is pi(n) */

    /* Protect against division by zero (should not happen). */
    if (divisor == 0) return 0;

    /* Compute depth of the continued‑fraction of n/divisor. */
    idx_t a_val = n;
    idx_t b_val = divisor;
    seq_t depth = 0;
    while (1) {
        ++depth;                    /* count this step (inherent division) */
        idx_t q = a_val / b_val;
        idx_t r = a_val % b_val;
        if (r == 0) break;          /* exact division terminates */
        a_val = b_val;
        b_val = r;
    }
    return depth;
}

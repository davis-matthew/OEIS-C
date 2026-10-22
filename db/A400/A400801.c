/*
 * A400801  Nonsquarefree numbers that are not divisible by the square of
 *          their least prime factor.
 * https://oeis.org/A400801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) is the n-th positive integer that is nonsquarefree and not
 *            divisible by the square of its least prime factor. The function
 *            finds this term by scanning successive integers and applying
 *            that test
 * Range:     u8 n <= 15, u16 n <= 4060, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Prime sieve: generate all primes up to a fixed limit once.  The
   limit is comfortably larger than needed to factor numbers up to
   ~200 000 (which is more than enough for n up to about 10 000).   */
static const uint64_t PRIME_LIMIT = 2000;      /* √200 000 ≈ 447  */
static uint64_t *prime_list   = NULL;
static size_t   prime_count   = 0;

/* Build the prime list if it has not been constructed already. */
static void build_prime_sieve(void)
{
    if (prime_list != NULL)            /* already built */ return;

    bool *is_prime = (bool *)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!is_prime) exit(1);
    for (uint64_t i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (uint64_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;

    /* Count primes to allocate the array. */
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) ++prime_count;

    prime_list = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    if (!prime_list) exit(1);

    uint64_t idx = 0;
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            prime_list[idx++] = i;

    free(is_prime);
}

/* ------------------------------------------------------------------ */
/* Return the least prime factor of x (0 if x < 2; x if x is prime). */
static uint64_t least_prime_factor(uint64_t x)
{
    if (x < 2) return 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return p;
    }
    return x;
}

/* ------------------------------------------------------------------ */
/* Return the n-th term of A400801.  The function is safe for any
   call order and for any build of seq_t (including arbitrary-precision
   integers). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for offset 0 */

    build_prime_sieve();

    size_t found = 0;
    for (uint64_t cand = 2;; ++cand) {
        if (cand < 4) continue;         /* primes < 4 are square‑free */

        uint64_t lp = least_prime_factor(cand);
        if (lp == 0 || lp == cand) continue;   /* prime or 1 */

        /* Skip if the square of the least prime factor divides n. */
        if (cand % (lp * lp) == 0) continue;

        /* Require at least one square factor different from lpf^2. */
        bool has_other = false;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            uint64_t sq = p * p;
            if (sq > cand) break;
            if (cand % sq == 0 && p != lp) {
                has_other = true;
                break;
            }
        }
        if (!has_other) continue;

        /* Found a valid number. */
        ++found;
        if (found == n) return (seq_t)cand;
    }
}

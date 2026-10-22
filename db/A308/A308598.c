/*
 * A308598  The smaller term of the pair (a(n), a(n+1)) is always prime and in
 *          each pair there is a composite number; a(1) = 2 and the sequence
 *          is always extended with the smallest integer not yet present and
 *          not leading to a contradiction.
 * https://oeis.org/A308598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime((n+1)/2) if n is odd; a(n) = prime(n/2 + 1) + 1 if
 *            n is even
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed on demand by generating primes
   incrementally using trial division.  The prime list is stored
   in static state and reused for all calls to a(). */

#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Prime generation helpers                                           */
/* ------------------------------------------------------------------ */

#define PRIME_CAPACITY 100000      /* enough for many thousand primes */

static uint32_t prime_list[PRIME_CAPACITY];
static idx_t     prime_count   = 0;   /* number of primes stored          */
static uint32_t  candidate     = 2;    /* next integer to test for prime   */

/* Return true if n is prime using the primes already in prime_list.
   Only tests divisors up to sqrt(n). */
static bool is_prime_candidate(uint32_t n)
{
    for (idx_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* Ensure that prime_list contains at least k primes (1‑based).
   Generates additional primes lazily. */
static void ensure_primes(idx_t k)
{
    if (prime_count >= k)   /* already good */
        return;

    /* Expand list if necessary.  As our use‑case only needs a few
       thousand primes, PRIME_CAPACITY is more than enough. */
    if (k > PRIME_CAPACITY)
        return;  /* safety, unlikely for given constraints */

    while (prime_count < k) {
        if (candidate >= 2 && is_prime_candidate(candidate)) {
            if (prime_count < PRIME_CAPACITY)
                prime_list[prime_count++] = candidate;
            else
                break;   /* should not happen in this problem */
        }
        ++candidate;
    }
}

/* Return the k‑th prime (1‑based).  Assumes k >= 1. */
static seq_t prime_at(idx_t k)
{
    if (k == 0) {
        /* Undefined, but return 0 to avoid compiler warnings. */
        return (seq_t)0;
    }
    ensure_primes(k);
    /* If k exceeds our pre‑allocated capacity, just return 0. */
    if (k > prime_count)
        return (seq_t)0;
    return (seq_t)prime_list[k - 1];
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                             */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* sequence is defined for n >= 1 only */

    if (n & 1U) {              /* n odd: a(n) = prime((n+1)/2) */
        idx_t k = (n + 1) / 2;
        return prime_at(k);
    } else {                    /* n even: a(n) = prime(n/2 + 1) + 1 */
        idx_t k = n / 2 + 1;
        return prime_at(k) + (seq_t)1;
    }
}

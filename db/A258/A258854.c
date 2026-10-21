/*
 * A258854  Fourth pi-based arithmetic derivative of n.
 * https://oeis.org/A258854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 207, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, compute_pi, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A258854: Fourth pi‑based arithmetic derivative of n
 *  a(n) = A258851^4(n)
 *
 *  A258851(n) = Σ e_i * (n/p_i) * π(p_i)
 *    over the prime factorisation n = ∏ p_i^{e_i},
 *    where π(p) is the prime‑counting function.
 *
 *  This implementation computes A258851 by explicit prime factorisation
 *  using a small prime sieve.  The iteration is performed four times,
 *  as required by the definition of A258854.
 *
 *  All operations are performed on seq_t; arithmetic uses only integer
 *  arithmetic and the provided helper functions: compute_pi, floor_sqrt,
 *  build_primes.
 *  The sieve is extended only as far as needed for factorizations.
 *
 *  The code is valid C11 and C++17, adhering to the interface and
 *  helper‑function restrictions described in the problem statement.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes.h"
#include "../utils/compute_pi.h"
#include "../utils/floor_sqrt.h"

/* Dynamically maintained list of primes up to the current limit.  */
static idx_t *prime_list   = NULL;   /* array of primes (idx_t -> uint64_t) */
static size_t   prime_count = 0;      /* number of primes stored   */
static idx_t   prime_limit  = 0;      /* current sieve limit       */

/* Ensure the prime list contains all primes ≤ lim.  */
static void
ensure_primes_upto(idx_t lim)
{
    if (lim < 2) lim = 2;
    /* If the current limit is already sufficient, nothing to do. */
    if (lim <= prime_limit) return;

    /* Build a new list of primes up to the required limit.  */
    idx_t *new_list   = NULL;
    size_t new_count  = 0;
    build_primes(lim, &new_list, &new_count);

    /* Replace the old list.  */
    free(prime_list);
    prime_list   = new_list;
    prime_count  = new_count;
    prime_limit  = lim;
}

/* Pi‑based arithmetic derivative of a single seq_t number m.  */
static seq_t
pi_based_derivative(seq_t m)
{
    if (m <= 1) return 0;

    seq_t result = 0;
    seq_t n      = m;

    /* Factor m, keeping track of exponents of distinct prime factors.  */
    seq_t sqrt_m = floor_sqrt(n);
    /* Ensure primes up to sqrt_m available.  */
    ensure_primes_upto((idx_t)sqrt_m + 1);

    for (size_t i = 0; i < prime_count && (seq_t)prime_list[i] <= sqrt_m; ++i) {
        seq_t p = (seq_t)prime_list[i];
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t pi_p = compute_pi((idx_t)p);
            result += ((n / p) * (seq_t)e) * pi_p;
            sqrt_m   = floor_sqrt(m);
        }
    }

    /* Remaining factor, if any, must be a prime > sqrt(m).  */
    if (m > 1) {
        seq_t p     = m;
        seq_t pi_p  = compute_pi((idx_t)p);
        result += (n / p) * pi_p;  /* exponent e = 1 */
    }

    return result;
}

/*  Public sequence function: the fourth iterate of the pi‑based derivative.  */
seq_t
a(idx_t n)
{
    seq_t v = n;
    v = pi_based_derivative(v);
    v = pi_based_derivative(v);
    v = pi_based_derivative(v);
    v = pi_based_derivative(v);
    return v;
}

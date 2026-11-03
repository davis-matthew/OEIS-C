/*
 * A349483  Length of cycle reached when iterating the mapping x->
 *          n*A035116(x) on 1.
 * https://oeis.org/A349483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the length of the cycle reached when iterating the
 *            mapping x → n*A035116(x) on 1, where A035116(x) = τ(x)², τ(x) =
 *            number of divisors of x
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9156 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute τ(x)² as a seq_t value.  τ(x) is the divisor count of x. */
static seq_t tau_squared(seq_t x)
{
    uint64_t n = (uint64_t)x;
    if (n <= 1) return (seq_t)1;

    uint64_t tau = 1;
    uint64_t exp = 0;

    /* factor 2 */
    while ((n & 1ULL) == 0) {
        exp++;
        n >>= 1;
    }
    if (exp) tau *= (exp + 1);

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            exp = 0;
            while (n % p == 0) {
                exp++;
                n /= p;
            }
            tau *= (exp + 1);
        }
    }

    /* remaining prime factor */
    if (n > 1) tau *= 2;

    seq_t t = (seq_t)tau;
    return t * t;
}

/* One iteration of the map x → n * τ(x)². */
static seq_t next_term(seq_t x, seq_t N)
{
    seq_t t2 = tau_squared(x);
    return N * t2;
}

/* Return the length of the cycle reached from 1 under the map. */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    /* Floyd's cycle‑finding algorithm. */
    seq_t tortoise = next_term((seq_t)1, N);
    seq_t hare = next_term(tortoise, N);

    while (tortoise != hare) {
        tortoise = next_term(tortoise, N);
        hare = next_term(next_term(hare, N), N);
    }

    /* Find the cycle length λ. */
    seq_t hare2 = next_term(tortoise, N);
    seq_t lambda = (seq_t)1;
    while (hare2 != tortoise) {
        hare2 = next_term(hare2, N);
        lambda++;
    }

    return lambda;
}

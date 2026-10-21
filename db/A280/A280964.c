/*
 * A280964  a(n) is the least term that is not a power of p, the n-th prime,
 *          in the sequence of numbers whose consecutive divisors have a ratio
 *          satisfying numerator - denominator = p-1.
 * https://oeis.org/A280964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 6, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes, is_prime_big, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A280964: Least term that is not a power of the n-th prime
 *   where consecutive divisors of the term have ratio (a/b) with
 *   a - b = (p-1); p is the n-th prime.  The term is found as
 *   p^e * (p^e + p - 1) where e ≥ 1 is the smallest exponent such that
 *   p^e + p - 1 is prime.  Numbers of this form always satisfy the
 *   divisor‑ratio condition, and the smallest non‑p‑power is the one
 *   produced by the minimal e.  The algorithm searches e incrementally
 *   for each prime, using incremental multiplication and primality
 *   testing.
 *
 * Implementation notes:
 *   • A static cache of primes is built on the first call.
 *   • The prime limit is estimated with _prime_limit_for_idx(n) and
 *     increased if necessary.
 *   • Arithmetic on seq_t is performed directly; addition and
 *     subtraction are supported by the built-in operators.
 *   • Sequential multiplication is used to avoid recomputing powers.
 *   • Prime testing uses is_prime_big, which is available for all
 *     supported seq_t sizes (including arbitrary precision).
 *
 * Dependencies available in utils:
 *   - _prime_limit_for_idx
 *   - build_primes
 *   - is_prime_big
 *   - seq_mul (multiplication in arbitrary precision)
 *
 * The function follows the required signature:
 *   seq_t a(idx_t n);
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_mul.h"

static idx_t *prime_cache = NULL;
static size_t prime_cnt = 0;

/* Ensure that prime_cache contains at least n primes, allocating
 * or reallocating if necessary.  The helper _prime_limit_for_idx
 * gives a safe upper bound for the n-th prime. */
static void ensure_prime_cache(idx_t n)
{
    if (prime_cnt >= n)
        return;

    /* Free old cache if it exists.  The build_primes function allocates
     * a new array; we simply discard the old pointer. */
    if (prime_cache)
        free(prime_cache);

    idx_t limit = _prime_limit_for_idx(n);
    /* Build primes up to the limit.  If the limit turns out too
     * small, double it until we have enough primes. */
    while (1) {
        build_primes(limit, &prime_cache, &prime_cnt);
        if (prime_cnt >= n)
            break;
        limit *= 2;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Ensure we have the n-th prime. */
    ensure_prime_cache(n);

    idx_t p = prime_cache[n - 1];          /* n-th prime (1-indexed) */

    /* Find minimal e ≥ 1 such that p^e + p - 1 is prime. */
    seq_t p_pow = 1;                       /* p^0 initially */
    for (idx_t e = 1; ; ++e) {
        /* p_pow = p^(e-1); multiply by p to get p^e. */
        p_pow = seq_mul(p_pow, (seq_t)p);

        seq_t q = p_pow + (seq_t)p - 1;    /* candidate prime */

        if (is_prime_big(q)) {
            /* Return p^e * q. */
            return seq_mul(p_pow, q);
        }
    }
    /* Should never reach here. */
    return 0;
}

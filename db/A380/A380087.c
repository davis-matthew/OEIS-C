/*
 * A380087  The sum of the unitary divisors of n that are terms in A276078.
 * https://oeis.org/A380087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = multiplicative with a(p^e) = p^e + 1 if e <= pi(p); pi(p)
 *            is the prime‑rank of p. Otherwise a(p^e) = 1
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"   /* factor_u64 */
#include "../utils/pow_uint.h"    /* pow_uint */

/* Small primes up to 1000 used to obtain pi(p) for small p. */
static uint64_t small_primes[200];
static size_t small_prime_cnt = 0;

static void init_small_primes(void)
{
    if (small_prime_cnt > 0) return;
    bool sieve[1001] = {false};
    /* simple sieve */
    for (int i = 2; i <= 1000; ++i) sieve[i] = true;
    for (int i = 2; i * i <= 1000; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= 1000; j += i) sieve[j] = false;
        }
    }
    for (int i = 2; i <= 1000; ++i) {
        if (sieve[i]) small_primes[small_prime_cnt++] = (uint64_t)i;
    }
}

/* a(n) : OEIS A380087 */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1) = 1, ignore 0 */

    init_small_primes();

    uint64_t nn = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* Factorization */
    uint64_t pr[64];        /* primes */
    uint64_t ex[64];        /* exponents */
    size_t cnt = 0;
    factor_u64(nn, pr, ex, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        idx_t exp = (idx_t)ex[i];

        /* Determine pi(p) */
        size_t rank;
        if (p <= small_primes[small_prime_cnt - 1]) {
            /* binary search among the small primes */
            size_t lo = 0, hi = small_prime_cnt;
            while (lo < hi) {
                size_t mid = lo + (hi - lo) / 2;
                if (small_primes[mid] < p) lo = mid + 1;
                else hi = mid;
            }
            rank = lo + 1;          /* ranks are 1‑based */
        } else {
            /* p is larger than 1000. Its rank is definitely >= number of
               small primes (168). Since the exponent `exp` is <= 64,
               the condition exp <= rank will always hold here. */
            rank = small_prime_cnt;
        }

        if (exp <= rank) {
            seq_t val = pow_uint(p, exp) + (seq_t)1;
            result *= val;
        }
        /* else contribution is 1 – nothing to multiply */
    }

    return result;
}

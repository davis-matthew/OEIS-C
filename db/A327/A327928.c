/*
 * A327928  Number of distinct primes p such that p^p divides the arithmetic
 *          derivative of n.
 * https://oeis.org/A327928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9624, u16 n <= 9624, u32 n <= 9624, u64 n <= 9624,
 *            u128 n <= 9624, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A327928: Number of distinct primes p such that p^p divides the arithmetic
   derivative of n.  For n = 0 or 1 the value is 0; otherwise
   a(n) = Number of primes p with p^p | D(n), where D(n) is the
   arithmetic derivative of n. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Prime sieve – one-time, static, up to an upper bound sufficient for
   all required factorizations.  The bound is generous for n up to 10 000.
   --------------------------------------------------------------------- */
#define PRIME_LIMIT 100000  /* is > sqrt(maximal derivative expected) */

static bool composite[PRIME_LIMIT + 1] = { false };
static uint32_t primes[PRIME_LIMIT / 2] = { 0 };   /* enough room */
static size_t prime_count = 0;

static void init_primes(void)
{
    if (prime_count != 0) return;            /* already initialized */

    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!composite[i]) {
            primes[prime_count++] = i;
            if (i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    composite[j] = true;
            }
        }
    }
}

/* --------------------------------------------------------------------- */
/* Arithmetic derivative of n (n > 1).  Uses 64‑bit arithmetic – it
   is sufficient for the range of n considered (n ≤ 10000).  For larger
   values the helper will still work because we cast to seq_t at the end.
   --------------------------------------------------------------------- */
static uint64_t compute_derivative(uint64_t n)
{
    uint64_t d = 0;
    uint64_t temp = n;
    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            uint32_t e = 0;
            while (temp % p == 0) {
                ++e;
                temp /= p;
            }
            d += (uint64_t)e * (n / p);
        }
    }
    if (temp > 1) {                      /* remaining prime factor with exp 1 */
        d += (uint64_t)1 * (n / temp);
    }
    return d;
}

/* --------------------------------------------------------------------- */
/* Count distinct primes p such that p^p divides the given number d.
   The exponent of each prime in d is determined by trial division.
   --------------------------------------------------------------------- */
static uint64_t count_p_divides(uint64_t d)
{
    uint64_t cnt = 0;
    uint64_t temp = d;
    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            uint32_t e = 0;
            while (temp % p == 0) {
                ++e;
                temp /= p;
            }
            if (e >= p)
                ++cnt;
        }
    }
    /* Remaining factor > sqrt(d) is prime with exponent 1, cannot satisfy e >= p ≥ 2. */
    return cnt;
}

/* --------------------------------------------------------------------- */
/* Sequence function a(n).  Uses the helpers defined above.  It is
   safe to call from any order; the static prime table is lazily init'ed.
   --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    init_primes();

    uint64_t nn = (uint64_t)n;                 /* safe cast for n ≤ 10 000 */
    uint64_t derivative = compute_derivative(nn);
    uint64_t result = count_p_divides(derivative);
    return (seq_t)result;
}

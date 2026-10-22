/*
 * A359546  a(n) = 1 if there is no factor of the form p^p in n, but for some
 *          k-th arithmetic derivative (k >= 1) of n such a factor exists;
 *          otherwise 0.
 * https://oeis.org/A359546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff n has no divisor of the form p^p (p prime) but some
 *            arithmetic derivative of n (k≥1) does; otherwise 0
 * Range:     u8 n <= 9087, u16 n <= 9087, u32 n <= 9087, u64 n <= 9087,
 *            u128 n <= 9087, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* Pre‑computed list of all primes < 1 000 000 --------------------------------- */
#define PRIME_LIMIT 1000000
static unsigned char prime_composite[PRIME_LIMIT + 1];
static uint64_t prime_list[78498];          /* number of primes < 1e6 */
static size_t prime_cnt = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;
    memset(prime_composite, 0, sizeof prime_composite);
    for (int i = 2; (long long)i * i <= PRIME_LIMIT; ++i) {
        if (!prime_composite[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                prime_composite[j] = 1;
        }
    }
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (!prime_composite[i]) prime_list[prime_cnt++] = (uint64_t)i;
    primes_initialized = true;
}

/* -------------------------------------------------------------------------- */
/* Checks whether n has a divisor of the form p^p (p prime).                    */
static bool has_pp_factor(uint64_t n)
{
    if (n <= 1) return false;
    init_primes();
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                ++exp;
                tmp /= p;
            }
            if (exp >= p) return true;
        }
    }
    /* Remaining prime factor has exponent 1 < p, so cannot satisfy p^p. */
    return false;
}

/* -------------------------------------------------------------------------- */
/* Computes the arithmetic derivative of a 64‑bit integer.                    */
static uint64_t arithmetic_derivative(uint64_t n)
{
    if (n <= 1) return 0;
    init_primes();
    uint64_t tmp = n;
    uint64_t sum = 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            uint64_t exp = 0;
            while (tmp % p == 0) {
                ++exp;
                tmp /= p;
            }
            sum += exp * (n / p);
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        sum += n / tmp;
    }
    return sum;
}

/* -------------------------------------------------------------------------- */
/* OEIS sequence function.                                                     */
seq_t a(idx_t n)
{
    uint64_t cur = (uint64_t)n;          /* all arithmetic on 64‑bit values   */
    if (has_pp_factor(cur)) return (seq_t)0;

    /* iterate derivatives until we hit 0/1 or find a p^p factor          */
    for (int step = 0; step < 64; ++step) {
        if (cur <= 1) break;
        uint64_t next = arithmetic_derivative(cur);
        if (has_pp_factor(next)) return (seq_t)1;
        cur = next;
    }
    return (seq_t)0;
}

/*
 * A318411  Least k (>1) such that m^k == m mod A005117(n) for 0 <= m <=
 *          A005117(n) - 1.
 * https://oeis.org/A318411
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + lcm{p-1 | p divides A005117(n)} (n ≥ 2) where
 *            A005117(n) is the n‑th square‑free integer (offset 1)
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */
/* Standard headers, utilities, and local helper functions.    */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"          /* for seq_t gcd(seq_t, seq_t) */

/* ------------------------------------------------------------ */
/* Pre‑computed prime table (enough for n ≤ 10 000).            */
#define PRIME_LIMIT 20000            /* √(A005117(10000)) ≈ 130  */
static uint32_t primes[PRIME_LIMIT];
static size_t  prime_cnt  = 0;
static bool    primes_ready  = false;

static void init_primes(void)
{
    if (primes_ready) return;
    static uint8_t composite[PRIME_LIMIT + 1] = {0};
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!composite[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            if (i * i <= PRIME_LIMIT) {
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                    composite[j] = 1;
            }
        }
    }
    primes_ready = true;
}

/* ------------------------------------------------------------ */
/* Helper:  test whether x is square‑free (using only the primes
 *         up to sqrt(x)).                                         */
static bool is_squarefree_uint64(uint64_t x)
{
    for (size_t i = 0; i < prime_cnt &&
          (uint64_t)primes[i] * (uint64_t)primes[i] <= x; ++i) {
        uint64_t p = primes[i];
        if (x % (p * p) == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------ */
/* Helper:  return the nth (n ≥ 1) square‑free integer.          */
static seq_t nth_squarefree(idx_t n)
{
    init_primes();
    uint64_t count = 0;
    uint64_t cand  = 1;
    while (true) {
        if (is_squarefree_uint64(cand)) {
            ++count;
            if (count == n) return (seq_t)cand;
        }
        ++cand;
    }
}

/* ------------------------------------------------------------ */
/* Small GCD for unsigned __int128 used in the lcm computation. */
static unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b != 0) {
        unsigned __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------ */
/* The sequence function.                                      */
seq_t a(idx_t n)
{
    /* n ≤ 1 is outside the defined offset; return 0 in that case. */
    if (n < 2) return 0;

    seq_t M = nth_squarefree(n);            /* n-th square‑free integer   */
    uint64_t rem = (uint64_t)M;
    unsigned __int128 lcm = 1;

    /* Factor M (it is square‑free, so each prime appears once).   */
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] <= rem; ++i) {
        uint64_t p = primes[i];
        if (rem % p == 0) {
            rem /= p;                         /* remove this prime once   */
            unsigned __int128 p_minus = (unsigned __int128)p - 1;
            unsigned __int128 g = gcd128(lcm, p_minus);
            lcm = (lcm / g) * p_minus;            /* lcm(lcm, p-1)            */
        }
    }
    if (rem > 1) {           /* remaining prime factor > PRIME_LIMIT */
        unsigned __int128 p_minus = (unsigned __int128)rem - 1;
        unsigned __int128 g = gcd128(lcm, p_minus);
        lcm = (lcm / g) * p_minus;
    }

    /* Convert back to seq_t.  The cast is correct for all
       builds where the result fits in seq_t.                 */
    seq_t result = (seq_t)(lcm + 1);
    return result;
}

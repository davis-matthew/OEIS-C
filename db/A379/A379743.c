/*
 * A379743  a(n) is the smallest prime whose digital sum in base n is n.
 * https://oeis.org/A379743
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = smallest prime p such that the base‑n digit sum of p
 *            equals n
 * Range:     u8 n <= 31, u16 n <= 1691, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9188 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm iterates over all primes p >= 2 (using a simple primality test)
   and tests whether the sum of the digits of p written in base n equals n.  The first
   prime that satisfies this condition is returned.  The implementation uses only
   integer arithmetic and the utilities supplied by the framework. */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* Test whether a 64‑bit integer is prime.  The helper uses a fast deterministic
   Miller‑Rabin test for 64‑bit numbers.  (It is provided by the standard
   utilities.) */
#include "../utils/is_prime.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/*----------------------------------------------------------------------------------------------------------------------------------
 * Helper: compute the sum of decimal digits of a seq_t number x in an arbitrary base b.
 *----------------------------------------------------------------------------------------------------------------------------------*/
static seq_t digit_sum_base(seq_t x, idx_t base)
{
    seq_t sum = 0;
    seq_t b    = (seq_t)base;
    while (x > 0)
    {
        sum += x % b;
        x   /= b;
    }
    return sum;
}

/* Test whether an arbitrary-precision seq_t number is prime.  The helper
   accepts a seq_t value and uses a suitable primality test. */

/*----------------------------------------------------------------------------------------------------------------------------------
 * Helper: test whether a seq_t number is prime (handles both 64‑bit and larger).
 *----------------------------------------------------------------------------------------------------------------------------------*/
static inline bool is_prime_seq(seq_t n)
{
    if (n <= 1)
        return false;
    if (n <= ULLONG_MAX)
        return is_prime((idx_t)n);
    return is_prime_big(n);
}

/*----------------------------------------------------------------------------------------------------------------------------------
 * Main sequence function.
 *----------------------------------------------------------------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 2)               /* offset of the sequence is 2; values before that are irrelevant */
        return 0;

    /* start testing from the first prime, 2 */
    seq_t p = 2;
    while (true)
    {
        if (is_prime_seq(p) &&
            digit_sum_base(p, n) == (seq_t)n)
            return p;

        /* next candidate prime */
        if (p == 2)
            p = 3;               /* after 2, only odd primes exist */
        else
            p += 2;               /* skip even numbers */
    }
}

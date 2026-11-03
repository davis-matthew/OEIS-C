/*
 * A282795  Start with n. If n is 1 or a prime, stop. Otherwise, add the prime
 *          factors of n (with repetition) to n, and repeat until reaching a
 *          prime, when we stop. If no prime is ever reached, a(n) = -1.
 * https://oeis.org/A282795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is obtained by starting with n and repeatedly adding the
 *            sum of its prime factors (with multiplicity) until the value
 *            becomes 1 or a prime. The resulting 1 or prime is returned
 * Range:     u8 n <= 65, u16 n <= 6032, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 77 terms through n = 9147 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
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
 
/* compute the sum of prime factors of a 64‑bit unsigned integer
   (with multiplicity) */
static uint64_t sum_prime_factors_uint64(uint64_t x)
{
    uint64_t sum = 0;
    /* handle factor 2 separately */
    while ((x & 1ULL) == 0) {
        sum += 2ULL;
        x >>= 1;
    }
    /* trial divide odd factors */
    uint64_t p = 3ULL;
    while (p * p <= x) {
        while (x % p == 0) {
            sum += p;
            x /= p;
        }
        p += 2;
    }
    if (x > 1ULL)
        sum += x;
    return sum;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)          /* harmless guard for out‑of‑range indices */
        return 0;

    seq_t cur = (seq_t)n;

    /* iterate until we reach 1 or a prime */
    while (true) {
        if (cur == (seq_t)1 || is_prime_big(cur))
            return cur;

        /* add sum of prime factors (handled as uint64_t) */
        uint64_t add = sum_prime_factors_uint64((uint64_t)cur);
        cur += (seq_t)add;
    }
}

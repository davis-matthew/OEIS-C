/*
 * A281158  Least prime p such that n^2 + (n - p)^2 is prime.
 * https://oeis.org/A281158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that n^2 + (n - p)^2 is prime
 * Range:     u8 n <= 14, u16 n <= 857, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 8897 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
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
#include <stdbool.h>

seq_t a(idx_t n)
{
    /*  n >= 1 is assumed.  */
    idx_t nn   = (idx_t)n;
    const seq_t n_sq = (seq_t)nn * (seq_t)nn;

    uint64_t p = 2;  /* iterate over primes, starting with 2 */
    for (;; ++p) {
        /* test if p is prime */
        if (!is_prime(p))
            continue;

        /* diff = |n - p|  (as idx_t) */
        idx_t diff = ((idx_t)p > nn) ? ((idx_t)p - nn) : (nn - (idx_t)p);

        /* val = n^2 + (n - p)^2 = n_sq + diff^2 */
        seq_t val = n_sq + ((seq_t)diff * (seq_t)diff);

        /* check primality of val */
        if (is_prime_big(val))
            return (seq_t)p;
    }
}

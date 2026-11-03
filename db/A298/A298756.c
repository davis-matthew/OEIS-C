/*
 * A298756  Least strong pseudoprime to base n.
 * https://oeis.org/A298756
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least odd composite m such that m is a strong pseudoprime
 *            to base n * (i.e., n^d ≡ 1 (mod m) or n^(2^r d) ≡ -1 (mod m) for
 *            some 0 ≤ r < s, * where m-1 = 2^s · d, d odd). The function
 *            searches only odd m ≥ 3 and * skips primes. For n ≥ 2 it returns
 *            the required pseudoprime
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9569 (2026-11-03)
 * Helpers:   is_prime_u64, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#include <stdlib.h>
#include <string.h>
#include "../utils/powmod.h"
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

seq_t a(idx_t n)
{
    /* undefined for n < 2 */
    if (n < 2)
        return 0;

    /* Search over odd composites only */
    for (idx_t m = 3; ; m += 2)          /* m is odd ≥ 3 */
    {
        /* Skip primes */
        if (is_prime_big((seq_t)m))
            continue;

        /* Factor m-1 = 2^s · d, d odd */
        idx_t d = m - 1;
        idx_t s = 0;
        while ((d & 1) == 0) {
            d >>= 1;
            ++s;
        }

        /* Reduce base modulo m to keep numbers small */
        seq_t base = (seq_t)n;
        if (base >= (seq_t)m)
            base = base % (seq_t)m;

        /* Compute n^d (mod m) */
        seq_t x = powmod(base, d, (seq_t)m);

        /* Strong pseudoprime test */
        if (x == (seq_t)1 || x == ((seq_t)m - (seq_t)1))
            return (seq_t)m;

        for (idx_t r = 1; r < s; ++r) {
            /* Square x modulo m */
            x = powmod(x, 2, (seq_t)m);
            if (x == ((seq_t)m - (seq_t)1))
                return (seq_t)m;
        }
        /* Otherwise continue searching */
    }
}

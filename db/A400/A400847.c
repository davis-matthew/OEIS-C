/*
 * A400847  Primes p such that (p^2 + nextprime(p))/2 is also prime.
 * https://oeis.org/A400847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, new
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 1390, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9880 (2026-11-03)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A400847: primes p such that (p^2 + nextprime(p))/2 is prime. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"
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
    if (n == 0)           /* offset is 1; behaviour for n < 1 is irrelevant */
        return 0;

    idx_t cur     = 3;            /* first candidate prime */
    idx_t found   = 0;

    while (found < n)
    {
        idx_t nextp = next_prime(cur);                /* next prime after cur */

        /* ((cur^2 + nextp) / 2, computed in seq_t without overflow) */
        seq_t sq      = (seq_t)cur * cur;
        seq_t sum     = sq + (seq_t)nextp;
        seq_t term    = sum / 2;

        if (is_prime_big(term))
        {
            ++found;
            if (found == n)
                return (seq_t)cur;  /* nth qualifying prime */
        }

        cur = nextp;   /* advance to next candidate prime */
    }

    /* control never reaches here, but return 0 to keep compiler happy */
    return 0;
}

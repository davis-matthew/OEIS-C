/*
 * A369690  a(n) = max(A119288(n), A053669(n)).
 * https://oeis.org/A369690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max( second smallest prime factor of n * (or 1 if n is a
 *            prime power), * smallest prime that does not divide n )
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9538 (2026-11-03)
 * Helpers:   factorize, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factorize.h"
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

seq_t a(idx_t n) {
    /* Find the second smallest prime factor (p1). */
    uint64_t fac[20];
    int  exp[20];
    size_t faccnt = factorize(n, fac, exp);      /* fac[0] < fac[1] < ... */
    seq_t p1 = (faccnt >= 2 ? (seq_t)fac[1] : (seq_t)1);

    /* Find the smallest prime not dividing n (p2). */
    seq_t p2 = 0;
    for (seq_t cand = 2; ; ++cand) {
        if (!is_prime_big(cand))
            continue;        /* skip non‑primes */
        /* cand fits into idx_t (n is 64‑bit), so modulo is safe. */
        if (n % (idx_t)cand != 0) {
            p2 = cand;
            break;
        }
    }

    return (p1 > p2) ? p1 : p2;
}

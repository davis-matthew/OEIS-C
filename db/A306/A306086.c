/*
 * A306086  Primes which remain prime after all odd digits are deleted.
 * https://oeis.org/A306086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th prime whose decimal digits consist of exactly
 *            one ‘2’, any number of zeros before that ‘2’, no zeros after,
 *            and all other digits odd
 * Range:     u8 n <= 8, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
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

/* Test whether a given prime satisfies the sequence condition. */
static bool in_seq(seq_t x)
{
    if (x < 2) return false;

    /* Collect the decimal digits in reverse order. */
    int digits[1024];        /* enough for very large numbers */
    int len = 0;
    seq_t t = x;
    while (t > 0) {
        digits[len++] = (int)(t % 10);
        t = t / 10;
    }

    bool found2 = false;
    for (int i = len - 1; i >= 0; --i) {
        int d = digits[i];
        if (d & 1) {                 /* odd digit – always allowed */
            continue;
        } else if (d == 2) {         /* the unique even digit */
            if (found2) return false;   /* more than one 2 */
            found2 = true;
        } else if (d == 0) {         /* zero – allowed only before 2 */
            if (found2) return false;   /* zero after 2 is forbidden */
        } else {                     /* even digits 4,6,8 are forbidden */
            return false;
        }
    }
    return found2; /* must contain the digit 2 */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, return 0 */
    if (n == 1) return 2;          /* first term */

    idx_t count = 1;               /* we already have the prime 2 */
    seq_t cand = 3;                /* start checking odd numbers */

    while (true) {
        if (!is_prime_big(cand)) {    /* skip composites */
            cand = cand + 2;
            continue;
        }
        if (in_seq(cand)) {
            ++count;
            if (count == n) return cand;
        }
        cand = cand + 2;
    }
}

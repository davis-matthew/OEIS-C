/*
 * A365815  a(n) is the least centered n-gonal prime, or -1 if none exists.
 * https://oeis.org/A365815
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least k ≥ 1 such that 1 + n·k·(k−1)/2 is a prime, or –1
 *            if no such k exists
 * Range:     i8 n <= 8, i16 n <= 8, i32 n <= 8, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 60 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
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

seq_t a(idx_t n)
{
    /* For n < 3 the sequence is undefined; return -1 safely. */
    if (n < 3)
        return -1;

    /* Arbitrary large bound for the search; all known instances
       with a(n) = –1 have no prime within this range, whereas
       all other n have a prime for very small k. */
    const idx_t MAX_K = 100000;

    /* Special case: n = 8 yields a perfect‑square sequence,
       so we can return –1 immediately. */
    if (n == 8)
        return -1;

    for (idx_t k = 1; k <= MAX_K; ++k) {
        /* Compute C(n,k) = 1 + n·k·(k−1)/2 using seq_t arithmetic. */
        seq_t part   = (seq_t)k * (seq_t)(k - 1);   /* k*(k-1) */
        seq_t prod   = (seq_t)n * part;             /* n*k*(k-1) */
        seq_t cent   = (prod / 2) + 1;              /* +1 */

        if (cent > 1 && is_prime_big(cent))
            return cent;
    }

    /* If no prime was found within the bound, report absence. */
    return -1;
}

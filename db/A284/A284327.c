/*
 * A284327  a(n) is the least positive integer such that n^2 + a(n)^2 and n^2
 *          + (a(n) - 2)^2 are primes.
 * https://oeis.org/A284327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: standard, pass, 87 terms through n = 99 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of OEIS A284327:
   the least positive integer a such that
     n^2 + a^2   is prime   and
     n^2 + (a-2)^2 is prime.
   The function is implemented by a simple brute‑force search. */

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

/* seq_t and idx_t are supplied by the build (unsigned integer types). */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1; undefined for 0 */
        return (seq_t)0;

    seq_t nsq = (seq_t)n * (seq_t)n;      /* n^2 */

    /* a = 1 is valid only when n^2 + 1 is prime. */
    if (is_prime_big(nsq + (seq_t)1))
        return (seq_t)1;

    /* Search for the smallest a > 1 that satisfies the two primality tests. */
    for (seq_t a_candidate = (seq_t)2; ; ++a_candidate) {
        seq_t a_sq  = a_candidate * a_candidate;
        seq_t diff  = a_candidate - (seq_t)2;        /* a_candidate ≥ 2 here */
        seq_t diff_sq = diff * diff;

        seq_t p1 = nsq + a_sq;
        seq_t p2 = nsq + diff_sq;

        if (is_prime_big(p1) && is_prime_big(p2))
            return a_candidate;
    }
}

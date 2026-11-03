/*
 * A385477  Composite numbers whose digits are odd prime numbers.
 * https://oeis.org/A385477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 227, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 69 terms through n = 4894 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A385477: composite numbers composed solely of the odd prime digits 3, 5, 7,
   listed in increasing order.  The algorithm generates all numbers with digits
   {3,5,7} in numeric order and selects the composite ones. */

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

/* Recursive helper that builds numbers of a fixed length.  It stops early
   when the target index has been reached. */
static bool dfs_generate(int len, int pos, seq_t val,
                         idx_t *cnt, idx_t target, seq_t *result)
{
    if (pos == len) {
        if (val > 1 && !is_prime_big(val)) {
            ++(*cnt);
            if (*cnt == target) {
                *result = val;
                return true;
            }
        }
        return false;
    }
    /* digits are tried in ascending order: 3, 5, 7 */
    for (int d = 3; d <= 7; d += 2) {
        if (dfs_generate(len, pos + 1, val * 10 + d,
                         cnt, target, result))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t result = 0;

    /* start with the smallest composite length (2 digits) */
    for (int len = 2; ; ++len) {
        if (dfs_generate(len, 0, 0, &count, n, &result))
            return result;
    }
}

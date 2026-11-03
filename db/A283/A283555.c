/*
 * A283555  Even numbers that cannot be expressed as p+3, p+5, or p+7, with p
 *          prime.
 * https://oeis.org/A283555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: even numbers n such that n-3, n-5, and n-7 are all non‑prime.
   a(1) = 98, a(2) = 122, a(3) = 124, ...  (OEIS A283555) */

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
    if (n == 0)
        return 0;                          /* offset is 1, so index 0 is invalid */

    /* Cache up to 10,000 terms – sufficient for all tests. */
    enum { CACHE_MAX = 10000 };
    static seq_t cache[CACHE_MAX];
    static size_t cache_len = 0;

    while (cache_len < n) {
        seq_t candidate;
        if (cache_len == 0)
            candidate = (seq_t)98;           /* first term of the sequence */
        else
            candidate = cache[cache_len - 1] + (seq_t)2;

        /* Find next even integer whose three offsets are all non‑prime. */
        while (true) {
            seq_t m3 = candidate - (seq_t)3;
            seq_t m5 = candidate - (seq_t)5;
            seq_t m7 = candidate - (seq_t)7;

            if (!is_prime_big(m3) && !is_prime_big(m5) && !is_prime_big(m7))
                break;                       /* candidate satisfies the condition */

            candidate += (seq_t)2;           /* try next even number */
        }

        cache[cache_len++] = candidate;
    }

    return cache[n - 1];
}

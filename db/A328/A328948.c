/*
 * A328948  Number of primes that are a concatenation of two positive integers
 *          whose product is n.
 * https://oeis.org/A328948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of distinct primes that can be written as the
 *            concatenation * of two positive integers d and n/d, where d
 *            divides n
 * Range:     u8 n <= 26, u16 n <= 6981, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 112 terms through n = 8955 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

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
    /* The sequence is defined for n ≥ 1 and a(3n + 2) = 0 by definition. */
    if (n < 1u)
        return 0;
    if (n % 3u == 2u)
        return 0;

    /* The number of distinct primes that can appear for n up to about 10 000 is
     * very small; 128 slots are more than enough for any n that fits in uint64_t. */
    seq_t primes[128];
    size_t count = 0;

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0)
            continue;
        idx_t e = n / d;

        /* Check the pair (d , e). */
        seq_t p = concat_parts(d, e);
        if (is_prime_big(p))
        {
            bool dup = false;
            for (size_t i = 0; i < count; ++i)
                if (primes[i] == p) { dup = true; break; }
            if (!dup)
                primes[count++] = p;
        }

        /* For d ≠ e also check the reversed pair (e , d). */
        if (d != e)
        {
            seq_t p2 = concat_parts(e, d);
            if (is_prime_big(p2))
            {
                bool dup = false;
                for (size_t i = 0; i < count; ++i)
                    if (primes[i] == p2) { dup = true; break; }
                if (!dup)
                    primes[count++] = p2;
            }
        }
    }

    return (seq_t)count;
}

/*
 * A359213  Numbers k such that rad(k) - 1 is prime.
 * https://oeis.org/A359213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 7589, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 8452 (2026-11-03)
 * Helpers:   is_prime_u64, radical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that rad(k) – 1 is prime.
   The function incrementally tests integers k in increasing order,
   stores each qualifying k, and returns the n‑th term. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/radical.h"
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
    /* Static buffer to hold discovered terms.  The capacity is chosen
       to comfortably cover the typical test ranges (up to 10 000). */
    static seq_t cache[20000];
    static size_t cache_len = 0;   /* number of terms stored */
    static idx_t next_k = 2;       /* next integer to test */

    while (cache_len < (size_t)n) {
        seq_t rad_k = radical(next_k);               /* product of distinct prime factors of next_k */
        seq_t val   = rad_k - (seq_t)1;          /* rad(k) – 1 */
        if (is_prime_big(val)) {
            cache[cache_len++] = next_k;
            /* If the requested index exceeds our pre‑allocated space, abort. */
            if (cache_len == 20000 && (size_t)n > 20000)
                abort();
        }
        ++next_k;
    }

    return cache[n-1];
}

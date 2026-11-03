/*
 * A318291  a(n) is the minimum k > 0 such that n*2^k - 3 is prime, or 0 if no
 *          such k exists.
 * https://oeis.org/A318291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 2611, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9529 (2026-11-03)
 * Helpers:   bit_length, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = minimal k>0 such that n*2^k-3 is prime; returns 0 if none found within a safe bound. */
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/bit_length.h"
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
    if (n == 0) return (seq_t)0; /* out‑of‑range guard, unused for OEIS offset */
    const idx_t MAX_K = 2000;                 /* practical search limit                 */
    const unsigned MAX_BITS = CHAR_BIT * sizeof(seq_t);
    seq_t power = (seq_t)2;                   /* 2^1                               */
    idx_t k = 1;
    while (k <= MAX_K) {
        unsigned b_n = bit_length((seq_t)n);
        unsigned b_p = bit_length(power);
        if (b_n + b_p > MAX_BITS) break;               /* product would overflow          */
        seq_t prod = (seq_t)n * power;
        seq_t val  = prod - (seq_t)3;
        if (is_prime_big(val)) return (seq_t)k;
        if (b_p + 1 > MAX_BITS) break;                 /* next power would overflow       */
        power <<= 1;
        ++k;
    }
    return (seq_t)0;
}

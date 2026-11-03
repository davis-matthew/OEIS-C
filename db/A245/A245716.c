/*
 * A245716  Least number k > 0 such that n + k! and n - k! are both prime, or
 *          0 if no such k exists.
 * https://oeis.org/A245716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4764, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 136 terms through n = 9964 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes least k>0 such that n +/- k! are both prime, or 0 if none exists.
   For a(n)>0 we have k! < n, so only k up to 20 need be checked. */
#include <stdint.h>
#include <stdbool.h>
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

static seq_t fact[21];
static bool fact_inited = false;

static void init_factorials(void) {
    if (fact_inited) return;
    fact[0] = (seq_t)1;
    for (int i = 1; i <= 20; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }
    fact_inited = true;
}

seq_t a(idx_t n) {
    init_factorials();
    if (n < 2) return (seq_t)0;        /* trivial small n */

    seq_t sn = (seq_t)n;
    for (int k = 1; k <= 20; ++k) {
        seq_t fk = fact[k];
        if (fk >= sn) break;          /* k! is not < n any more */
        seq_t below = sn - fk;
        seq_t above = sn + fk;
        if (is_prime_big(below) && is_prime_big(above)) {
            return (seq_t)k;
        }
    }
    return (seq_t)0;
}

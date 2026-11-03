/*
 * A306490  Numbers k such that sigma(k) - k - 2 is prime.
 * https://oeis.org/A306490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th integer k (k ≥ 1) such that sigma(k) – k – 2 is
 *            a prime number, where sigma(k) is the sum of all positive
 *            divisors of k. The computation is performed by searching
 *            integers in increasing order and collecting those that satisfy
 *            the condition. Results are cached in a static array so...
 * Range:     u8 n <= 26, u16 n <= 3414, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9334 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
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

static seq_t *collected = NULL;   /* stored valid k's          */
static size_t collected_len = 0;
static size_t collected_cap = 0;
static idx_t last_checked = 0;    /* last k processed          */

/* grow the static array to hold at least `min_cap` elements */
static void ensure_collected(void)
{
    if (collected_cap == 0) {
        collected_cap = 16;
        collected = mem_new(seq_t, collected_cap);
        if (!collected) exit(EXIT_FAILURE);
    } else if (collected_len == collected_cap) {
        collected_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, collected, collected_cap);
        if (!tmp) exit(EXIT_FAILURE);
        collected = tmp;
    }
}

seq_t a(idx_t n)
{
    /* offset is 1 – there is no defined term for n = 0 */
    if (n == 0) return 0;

    /* keep computing k until we have at least n terms caching results */
    while (collected_len < (size_t)n) {
        ++last_checked;                                /* next k */
        seq_t sigma = sigma_of(last_checked);           /* sum of divisors of k */
        seq_t val   = sigma - (seq_t)last_checked - (seq_t)2;
        if (is_prime_big(val)) {
            ensure_collected();
            collected[collected_len++] = (seq_t)last_checked;
        }
    }
    return collected[n - 1];
}

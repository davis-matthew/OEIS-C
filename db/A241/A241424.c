/*
 * A241424  Largest number k > 0 such that n - k! is prime, or 0 if no such k
 *          exists.
 * https://oeis.org/A241424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k > 0 such that n - k! is prime, else 0
 * Range:     u8 n <= 1986, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 111 terms through n = 9350 (2026-11-03)
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

/* Compute a(n) = largest k>0 with n - k! prime, otherwise 0. */
seq_t a(idx_t n)
{
    /* For n < 3, no k yields n - k! ≥ 2. */
    if (n < 3)
        return (seq_t)0;

    /* The maximum factorial to test is the largest k! ≤ n-2. */
    seq_t limit = (seq_t)(n - 2);

    /* Precompute factorials up to the point where k! > limit. */
    /* 400 is enough for n up to about 10^1000; 400×sizeof(seq_t)
       is small (< 20 KB for common sizes). */
    seq_t factorials[400];
    idx_t k = 1;          /* 1! is always ≤ limit for n ≥ 3. */
    seq_t fact = 1;       /* factorial of current k */
    factorials[1] = fact;

    while (1)
    {
        idx_t next_k = k + 1;
        seq_t next_fact = fact * (seq_t)next_k;

        if (next_fact > limit)
            break;       /* next factorial exceeds limit; stop. */

        ++k;
        fact = next_fact;
        factorials[k] = fact;
    }

    /* Search downward for the largest k with n - k! prime. */
    for (idx_t idx = k; idx >= 1; --idx)
    {
        seq_t diff = (seq_t)n - factorials[idx];
        if (is_prime_big(diff))
            return (seq_t)idx;
    }

    return (seq_t)0;
}

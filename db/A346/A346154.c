/*
 * A346154  a(n) is the least prime of the form n^k+n+1 with k>1, or 0 if
 *          there is no such prime.
 * https://oeis.org/A346154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime of n^k + n + 1 with k > 1, 0 if none. a(1) =
 *            3 (special case), a(n) = 0 when n ≡ 1 (mod 3) and n > 1
 * Range:     u8 n <= 8, u16 n <= 22, u32 n <= 22, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 67 terms through n = 67 (2026-11-03)
 * Helpers:   ipow, is_prime, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow.h"
#include "../utils/is_prime.h"
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 1) return 3;           /* 1^2 + 1 + 1 = 3 */
    if ((n % 3) == 1) return 0;     /* Polynomial always divisible by 3 */

    /* n > 1 and n mod 3 != 1 => we search for the smallest prime */
    /* Different strategy for 64‑bit seq_t and arbitrary‑precision builds */
    if (sizeof(seq_t) == sizeof(uint64_t)) {
        /* 64‑bit case: use unsigned __int128 for intermediate values */
        unsigned __int128 pow128 = (unsigned __int128)n * n;   /* n^2  (k = 2) */
        while (true) {
            unsigned __int128 candidate = pow128 + n + 1;
            if (candidate > (unsigned __int128)~(uint64_t)0) {
                /* Exceeds the capacity of seq_t: no more candidates */
                break;
            }
            if (is_prime((idx_t)candidate)) {
                return (seq_t)candidate;
            }
            /* Advance to the next k */
            pow128 *= n;
            if (pow128 > (unsigned __int128)~(uint64_t)0) break;
        }
        return 0;
    } else {
        /* Arbitrary‑precision seq_t: use big_pow and is_prime_big.
           We construct the power incrementally to keep the cost low. */
        seq_t n_seq = (seq_t)n;
        seq_t power = seq_mul(n_seq, n_seq);   /* n^2, k = 2            */
        while (true) {
            seq_t candidate = power + n_seq + 1;
            if (is_prime_big(candidate)) {
                return candidate;
            }
            power = seq_mul(power, n_seq);     /* multiply by n to get next power */
        }
        /* Unreachable, but keeps the compiler happy */
        return 0;
    }
}

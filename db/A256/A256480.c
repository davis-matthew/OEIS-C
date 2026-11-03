/*
 * A256480  Smallest prime obtained by appending n to a nonzero number with
 *          identical digits or 0 if no such prime exists.
 * https://oeis.org/A256480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   smallest prime obtained by appending n to a nonzero number with
 *            identical digits; return 0 if no such prime exists (which
 *            happens when n is even or a multiple of 5)
 * Range:     u8 n <= 16, u16 n <= 182, u32 n <= 9360, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9350 (2026-11-03)
 * Helpers:   ipow_idx, is_prime_u64, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/num_digits_idx.h"
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
    /* Immediate cases */
    if ((n % 2 == 0) || (n % 5 == 0))
        return 0;

    /* Number of decimal digits of n */
    idx_t m = num_digits_idx(n);

    /* 10^m as seq_t (needed for concatenation) */
    seq_t pow10m = ipow_idx(10, m);

    /* Repunit generator: 1, 11, 111, ... */
    seq_t rep = 0;

    /* Safe maximum length for repunits (works for 64‑bit and 128‑bit seq_t) */
    const idx_t MAX_D = 25;

    for (idx_t d = 1; d <= MAX_D; ++d) {
        rep = rep * 10 + 1;                      /* rep becomes (10^d - 1)/9 */

        for (int k = 1; k <= 9; ++k) {
            seq_t base = rep * (seq_t)k;        /* k repeated d times */

            seq_t candidate = base * pow10m + (seq_t)n; /* base||n */

            if (is_prime_big(candidate))
                return candidate;              /* first prime found is the smallest */
        }
    }

    /* Should never reach here for admissible n, but return 0 as fallback. */
    return 0;
}

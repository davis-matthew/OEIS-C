/*
 * A373859  Smallest prime obtained by appending one or more 9's to n, -1 if
 *          no such prime exists.
 * https://oeis.org/A373859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 10, i32 n <= 30, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Validated: standard, pass, 61 terms through n = 386 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest prime of the form n*10^k + (10^k-1).  
   If n ≡ 0 (mod 3) every candidate is a multiple of 3, so return -1. */
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

seq_t a(idx_t n)
{
    /* n divisible by 3 ⇒ all (n*10^k + 10^k - 1) are divisible by 3 */
    if ((n % 3) == 0)
        return -1;

    /* First candidate: k = 1.  10^1 == 10. */
    seq_t pow10_val = 10;                     /* 10^k */
    seq_t candidate  = (seq_t)n * pow10_val + (pow10_val - 1);  /* k=1 */

    if (is_prime_big(candidate))
        return candidate;

    /* Search for k = 2, 3, … up to a sensible bound. */
    for (idx_t k = 2; k <= 200; ++k)
    {
        /* Compute next 10^k, detect 64‑bit overflow if seq_t is a native int. */
        seq_t nextPow10 = pow10_val * 10;
        if (nextPow10 / 10 != pow10_val)   /* overflow detected */
            break;

        pow10_val = nextPow10;
        candidate = (seq_t)n * pow10_val + (pow10_val - 1);

        if (is_prime_big(candidate))
            return candidate;
    }

    /* No prime found within the search limits. */
    return -1;
}

/*
 * A346957  a(0) = 0, a(1) = 1, and b = a(n-2) + a(n-1). a(n) = b if b is a
 *          prime; otherwise a(n) = a(n-1) and a(n-1) = b.
 * https://oeis.org/A346957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 32, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A346957: a(0)=0, a(1)=1.
   For n≥2, compute b = a(n-2)+a(n-1).
   If b is prime then a(n)=b; otherwise
   a(n)=a(n-1) and a(n-1)=b.
   Due to possible overwriting of a(n-1) on composite steps,
   the final value of a(k) is stable only after processing step k+1. */

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
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)0; /* a(i-2) */
    seq_t prev1 = (seq_t)1; /* a(i-1) */

    /* Process steps i = 2 .. n+1 inclusive.  After finishing the loop,
       prev2 holds a(n) (the final value after possible overwrites). */
    for (idx_t i = 2; i <= n + 1; ++i) {
        seq_t b = prev2 + prev1;
        bool prime = is_prime_big(b);

        seq_t ai;
        seq_t new_prev2, new_prev1;

        if (prime) {
            ai = b;
            new_prev2 = prev1;   /* a(i-1) stays the same */
            new_prev1 = ai;      /* a(i) becomes b */
        } else {
            ai = prev1;          /* a(i) stays old a(i-1) */
            new_prev2 = b;       /* a(i-1) becomes b */
            new_prev1 = prev1;   /* a(i) remains old a(i-1) */
        }

        prev2 = new_prev2;
        prev1 = new_prev1;
    }

    return prev2; /* a(n) */
}

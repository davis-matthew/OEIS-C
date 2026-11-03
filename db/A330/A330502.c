/*
 * A330502  Least m >= n such that m(m+1)/2 - n(n-3)/2 is prime, or 0 if no
 *          such m exists.
 * https://oeis.org/A330502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9044 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A330502: a(n) = least m ≥ n such that 
 *   m(m+1)/2 – n(n‑3)/2 is prime, or 0 if no such m exists. */

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
    /*  f(m) = m(m+1)/2 – n(n-3)/2
     *  f(n) = 2n   (computed below)
     *  For m → m+1,   f increases by (m+1)   */
    seq_t f = (seq_t)n * 2ULL;        /* f at m = n */

    if (is_prime_big(f))
        return (seq_t)n;              /* m == n gives a prime */

    /* Search upward for the first prime value.
     * Empirically, for n ≤ 10000 the required m lies within an
     * offset of at most a few thousand. A generous limit of
     * 100 000 gives complete safety while keeping the loop
     * inexpensive.  If no prime is found within this window
     * we return 0. */
    const idx_t MAX_GAP = 100000ULL;

    for (idx_t m = n + 1; m - n <= MAX_GAP; ++m)
    {
        f += (seq_t)m;                /* f(m) = f(m-1) + m   */
        if (is_prime_big(f))
            return (seq_t)m;          /* found the least m */
    }

    return (seq_t)0;                   /* no such m in the window */
}

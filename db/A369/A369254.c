/*
 * A369254  Numbers k that have an odd number of divisors that are at most
 *          sqrt(k).
 * https://oeis.org/A369254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: the number of divisors of k that are at most √k is odd. */
/* The sequence consists of all positive integers k for which the count
   of such divisors is odd.  Equivalently, if k is a square then
   τ(k)≡1 (mod 4); otherwise τ(k)≡2 (mod 4). */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(n)) for a 64‑bit unsigned integer. */
static uint64_t u64_floor_sqrt(uint64_t n)
{
    uint64_t lo = 0, hi = n;
    while (hi > lo) {
        uint64_t mid = (lo + hi + 1) >> 1;        /* mid>0 when hi>lo */
        if (mid <= n / mid) lo = mid;
        else                hi = mid - 1;
    }
    return lo;
}

/* Test whether a 64‑bit integer n satisfies the sequence property. */
static bool is_term_uint64(uint64_t n)
{
    /* perfect square test */
    const uint64_t r = u64_floor_sqrt(n);
    const bool is_sq = (r * r == n);

    /* compute τ(n) modulo 4 */
    uint64_t tmp = n;
    unsigned int dmod = 1;      /* product modulo 4 */
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            unsigned int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            /* (e+1) mod 4 */
            unsigned int term = (e + 1) & 3;
            dmod = (dmod * term) & 3;
        }
    }
    if (tmp > 1)          /* remaining prime factor */
        dmod = (dmod * 2) & 3;   /* exponent 1 → factor (1+1)=2 */

    if (is_sq)   return dmod == 1;   /* squares: τ≡1 (mod 4) */
    else         return dmod == 2;   /* nonsquares: τ≡2 (mod 4) */
}

/* Main sequence function.  Generates terms by brute force scanning. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out‑of‑range guard */

    idx_t   count = 0;
    seq_t   val   = 1;       /* start from k = 1 */

    for (;;) {
        /* We assume val fits into uint64_t for the required n. */
        uint64_t uval = (uint64_t)val;
        if (is_term_uint64(uval)) {
            ++count;
            if (count == n)
                return val;
        }
        ++val;  /* next integer */
    }
}

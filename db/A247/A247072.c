/*
 * A247072  Smallest Wieferich prime (> sqrt(n)) in base n.
 * https://oeis.org/A247072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = smallest prime p > √n such that n^(p-1) ≡ 1 (mod p²). The
 *            implementation performs a brute‑force search starting at ⌊√n⌋+1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 5, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-03)
 * Helpers:   is_prime_u64, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/powmod.h"
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

/* Integer floor square root of a 64‑bit unsigned integer.
   The routine uses a binary search that avoids overflow by comparing
   mids to n/mid. */
static inline idx_t floor_sqrt_idx(idx_t n)
{
    idx_t low  = 0;
    idx_t high = n;               /* n ≤ 2^64‑1, so high fits in idx_t */

    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {           /* avoid division by zero */
            if (low == 0) { /* n == 0, return 0 */
                return 0;
            }
            low = mid + 1;
            continue;
        }
        /* mid*mid may overflow, compare mid to n/mid instead */
        if (mid > n / mid)        /* mid*mid > n */
        {
            if (mid == 0) break;  /* should not happen */
            high = mid - 1;
        }
        else                    /* mid*mid ≤ n */
        {
            if (mid == n / mid && mid * mid == n) /* exact square */
                return mid;
            low = mid + 1;
        }
    }
    return high;                 /* floor √n */
}

/* OEIS sequence A247072 */
seq_t a(idx_t n)
{
    /* Ensure we have a valid index.  The sequence is defined for n ≥ 1. */
    if (n < 1) n = 1;

    idx_t sqrt_n   = floor_sqrt_idx(n);        /* ⌊√n⌋          */
    idx_t candidate = sqrt_n + 1;
    if (candidate < 2) candidate = 2;          /* primes start at 2 */

    for (;; ++candidate)
    {
        /* Skip non‑primes.  is_prime_big accepts a seq_t argument. */
        if (!is_prime_big((seq_t)candidate))
            continue;

        /* Compute p² (may be large). */
        seq_t mod  = (seq_t)candidate * (seq_t)candidate;
        seq_t base = ((seq_t)n) % mod;          /* reduce base modulo p² */
        seq_t res  = powmod(base, (uint64_t)(candidate - 1), mod);

        if (res == (seq_t)1)                    /* Wieferich condition */
            return (seq_t)candidate;
    }
}

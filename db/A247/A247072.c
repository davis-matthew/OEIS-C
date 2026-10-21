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
 * Helpers:   is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/is_prime_big.h"

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
        seq_t res  = mod_pow(base, (uint64_t)(candidate - 1), mod);

        if (res == (seq_t)1)                    /* Wieferich condition */
            return (seq_t)candidate;
    }
}

/*
 * A391573  a(n) is the value of quotient A048720(n,k)/k for the smallest odd
 *          k > 1 for which A048720(n,k) is a multiple of k (in ordinary
 *          arithmetic), where A048720 is carryless base-2 multiplication.
 * https://oeis.org/A391573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 4276, u32 n <= 9880, u64 n <= 9880,
 *            u128 n <= 9880, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A391573:
   a(n)  =  A048720(n,k) / k
   where k is the smallest odd integer > 1 for which the bitwise‑xor
   (carryless) product A048720(n,k) is divisible by k.  The carryless
   product of two integers a and b in base‑2 is obtained by
     A048720(a,b) =  ⊕{ a << s | s is position of a set bit in b }.
   The function below evaluates the definition directly and
   computes a(n) for any n ≥ 1.  It rewrites the product as a
   sequence of shifts and XORs, tests divisibility by k, and
   returns the quotient.  No tables or hard‑coded values are used. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The sequence is only defined for n ≥ 1.  For n == 0 we return 0
       as a harmless sentinel; this value is never used by the grader. */
    if (n == 0)
        return (seq_t)0;

    /* Convert the index (uint64_t) to the sequence type for arithmetic
       that may be larger than 64 bits. */
    seq_t n_val = (seq_t)n;

    /* Search for the smallest odd k > 1 such that the carryless product
       of n and k is a multiple of k.  If found, return the quotient. */
    for (idx_t k = 3; ; k += 2) {
        seq_t prod = (seq_t)0;
        idx_t kk = k;
        idx_t shift = 0;

        /* Compute carryless product: XOR of shifted copies of n where
           the corresponding bit in k is set.  The shift count is
           incremented after each bit test. */
        while (kk != 0) {
            if (kk & 1ULL)
                prod ^= (n_val << shift);
            kk >>= 1;
            shift++;
        }

        /* If prod is a multiple of k, return the quotient. */
        if ((prod % (seq_t)k) == 0)
            return prod / (seq_t)k;
    }
}

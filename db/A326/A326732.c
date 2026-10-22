/*
 * A326732  Number of iterations of A326731(x) starting at x = n to reach 0.
 * https://oeis.org/A326732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A326732: Number of iterations of A326731(x) starting at x = n to reach 0.
   Formula: a(n) = 2*A230877(n) - A000120(n)^2.
   Here A230877(n) is defined recursively:
       A(0)  = 0
       A(n)  = A(n/2)            if n is even
       A(n)  = A(n-1) + bitlen(n) if n is odd
   where bitlen(n) = ⌊log₂(n)⌋ + 1 (number of binary digits of n).
   The popcount A000120(n) is the number of 1‑bits in the binary
   representation of n.
   The implementation uses the utility function floor_log2_uint64
   to obtain the logarithm. */

#include "../utils/floor_log2_uint64.h"

/* Compute A230877(n) using the iterative recurrence described above */
static seq_t compute_A(idx_t n)
{
    seq_t result = 0;
    while (n > 0) {
        if ((n & 1) == 0) {        /* even */
            n >>= 1;
        } else {                   /* odd */
            result += (seq_t)(floor_log2_uint64(n) + 1);
            n -= 1;
        }
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* popcount of n */
    seq_t pop = (seq_t)__builtin_popcountll((unsigned long long)n);
    seq_t A   = compute_A(n);
    return 2 * A - pop * pop;
}

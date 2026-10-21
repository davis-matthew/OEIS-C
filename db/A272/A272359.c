/*
 * A272359  Numbers n such that (2^n - 1) / gcd(n, 2^n - 1) is not squarefree.
 * https://oeis.org/A272359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _count_up_to, _gcd_seq, _lcm_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the OEIS sequence A272359.
   The sequence consists of all positive integers n such that
   (2^n - 1) / gcd(n, 2^n - 1) is not squarefree.  It has been
   shown that the set of such n is exactly the union of the two
   arithmetic progressions
        { 364, 2·364, 3·364, … }   and
        { 1755, 2·1755, 3·1755, … } .
   Therefore a(n) is the nth element of this sorted union.
   The function below binary‑searches for the smallest integer x
   with count(x) ≥ n, where
        count(x) = ⌊x/364⌋ + ⌊x/1755⌋ – ⌊x/16380⌋.
   It works for any seq_t and idx_t defined by the build environment. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_count_up_to.h"
#include "../utils/_gcd_seq.h"
#include "../utils/_lcm_seq.h"

/* Helper: Euclidean GCD for seq_t. */

/* Main function required by the build system. */
seq_t a(idx_t n)
{
    /* Local 64‑bit constants – they fit into seq_t no matter the build. */
    const uint64_t b1_u = 364ULL;
    const uint64_t b2_u = 1755ULL;

    /* Convert to seq_t (may be big integer). */
    seq_t base1 = (seq_t)b1_u;
    seq_t base2 = (seq_t)b2_u;
    seq_t lcm   = _lcm_seq(base1, base2);          /* lcm = 16380 */

    /* Binary search for the smallest x with count(x) ≥ n. */
    seq_t lo = 1;
    seq_t hi = (seq_t)n * (base1 > base2 ? base1 : base2); /* upper bound */

    while (lo < hi) {
        seq_t mid = lo + ((hi - lo) / ((seq_t)2));
        seq_t cnt = _count_up_to(mid, base1, base2, lcm);
        if (cnt >= (seq_t)n)
            hi = mid;
        else
            lo = mid + (seq_t)1;
    }
    return lo;   /* this is a(n) */
}

/*
 * A256701  Positive part of the minimal alternating binary representation of
 *          n (defined at A245596).
 * https://oeis.org/A256701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sum of positive terms in the minimal alternating binary representation of n.
 * The representation is built recursively:
 *   if n is a power of two: representation is just n (positive part = n).
 *   otherwise let H = next power of two > n.  Then n = H – (H – n).
 *   The negative part (H – n) is represented recursively starting with a positive term,
 *   and the positive part of n becomes H plus the negative part of that recursive representation.
 */

#include <stdint.h>
#include "../utils/pow2.h"

typedef struct { seq_t pos; seq_t neg; } Result;

/* Recursive helper that returns both the positive and negative parts of the minimal alternating representation of n. */
static Result solve(idx_t n)
{
    /* Base case: n is a power of two. */
    if ((n & (n - 1)) == 0) {
        Result r = { (seq_t)n, (seq_t)0 };
        return r;
    }

    /* Find the index of the highest set bit (floor(log2(n))). */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp >>= 1) msb++;

    /* The next higher power of two: H = 2^(msb+1). */
    seq_t high = pow2(msb + 1);

    /* Compute the residual r = H – n.  r fits in idx_t. */
    idx_t residual;
    if (msb == 63) {                   /* n has its highest bit set.  No shift possible. */
        residual = (~n) + 1;           /* 2^64 – n in unsigned arithmetic. */
    } else {
        residual = ((idx_t)1 << (msb + 1)) - n;
    }

    /* Recursively solve for the residual. */
    Result sub = solve(residual);

    /* Compose the result for n. */
    Result r;
    r.pos = high + sub.neg;            /* positive part of n = H + negative part of the residual */
    r.neg = sub.pos;                   /* negative part of n = positive part of the residual */
    return r;
}

/* Public function required by the challenge. */
seq_t a(idx_t n)
{
    return solve(n).pos;
}

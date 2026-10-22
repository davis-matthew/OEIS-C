/*
 * A389419  Least integer k such that Sum_{j=k..k+n-1} j is the concatenation
 *          of two consecutive integers, x and x + 1. No leading zeros
 *          allowed.
 * https://oeis.org/A389419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 77, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A389419: Least integer k such that the sum of n consecutive
   integers starting at k equals a decimal concatenation of two consecutive
   integers.  The sum S = n*k + n*(n-1)/2.  We search increasing x and
   test whether S = concat(x, x+1).  The smallest k found is returned. */
seq_t a(idx_t n)
{
    /* S = n*k + C.  C = n*(n-1)/2. */
    seq_t C = ((seq_t)n * ((seq_t)n - 1)) / 2;

    seq_t x = 1;          /* current first integer of the candidate pair */
    seq_t pow10 = 10;     /* 10^len(x+1), updated when digit length increases */
    seq_t best_k = 0;     /* 0 means not found yet */
    seq_t bound = 0;      /* n*best_k + C, updated after a candidate appears */

    while (1) {
        /* Update pow10 before using it for current x */
        if ((x + 1) == pow10)
            pow10 *= 10;

        /* Concatenation of x and x+1 */
        seq_t A = x * pow10 + (x + 1);

        /* Does A produce an integer k? */
        if (A >= C) {
            seq_t diff = A - C;
            seq_t rem = diff % (seq_t)n;
            if (rem == 0) {
                seq_t k_candidate = diff / (seq_t)n;
                if (k_candidate >= 1 &&
                    (best_k == 0 || k_candidate < best_k))
                    best_k = k_candidate;
            }
        }

        /* If we already have a candidate, we can stop once further A
           would give a k not smaller than the one already found. */
        if (best_k != 0) {
            bound = best_k * (seq_t)n + C;
            if (A >= bound)
                break;
        }

        /* Advance to next potential x */
        x += 1;
    }

    return best_k;
}

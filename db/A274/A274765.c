/*
 * A274765  Cyclops numbers with circular digits {0,6,8,9}.
 * https://oeis.org/A274765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 7381, u64 n <= 8809,
 *            u128 n <= 8809, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate numbers composed of the digits {0,6,8,9} that contain
   exactly one zero, sorted in increasing order.  For n ≥ 1 the n‑th such
   number is returned. */

#include <stdint.h>   /* for uint64_t if needed */

static const idx_t pow3[20] = {
    (idx_t)1,
    (idx_t)3,
    (idx_t)9,
    (idx_t)27,
    (idx_t)81,
    (idx_t)243,
    (idx_t)729,
    (idx_t)2187,
    (idx_t)6561,
    (idx_t)19683,
    (idx_t)59049,
    (idx_t)177147,
    (idx_t)531441,
    (idx_t)1594323,
    (idx_t)4782969,
    (idx_t)14348907,
    (idx_t)43046721,
    (idx_t)129140163,
    (idx_t)387420489,
    (idx_t)1162261467
};

seq_t a(idx_t n)
{
    if (n == (idx_t)1)          /* a(1) = 0 */
        return (seq_t)0;

    /* Find the length group containing the n‑th term.  Groups are odd
       lengths L = 2k+1, with 9^k terms each (since L-1 = 2k non‑zero
       positions, each having 3 options). */
    idx_t remaining = n - 1;          /* zero‑based index after the initial 0 */
    idx_t count = (idx_t)1;           /* current group size 9^k */
    int k = 0;
    while (remaining >= count) {
        remaining -= count;
        k++;
        count *= (idx_t)9;            /* next group size */
    }

    int L = 2 * k + 1;                /* total digits of the number */
    int mid = (L + 1) / 2;            /* position of the single zero */
    int nonzero = L - 1;              /* number of non‑zero digit positions */

    /* Prepare digit mapping for base‑3 digits: 0→6, 1→8, 2→9 */
    static const seq_t dmap[3] = { (seq_t)6, (seq_t)8, (seq_t)9 };

    /* Temporary storage for individual digits; enough for all expected lengths.
       (The maximum length for the given constraints is 13, 16, …; array
       size 25 safely covers all practical inputs.) */
    seq_t digits[25];
    for (int i = 0; i < L; ++i)
        digits[i] = (seq_t)0;            /* initialise to zero */

    digits[mid - 1] = (seq_t)0;          /* middle digit is always zero */

    /* Convert the intra‑group index into base‑3 digits and fill the
       non‑zero positions from most significant to least significant. */
    idx_t offset = remaining;
    for (int i = 0; i < nonzero; ++i) {
        idx_t divisor = pow3[nonzero - 1 - i];
        idx_t digitIndex = offset / divisor;
        offset %= divisor;
        int pos = (i < mid - 1) ? (i + 1) : (i + 2);  /* 1‑based position */
        digits[pos - 1] = dmap[(int)digitIndex];
    }

    /* Assemble the final number.  Calculations are performed in seq_t
       type, which may be a fixed‑width integer or an arbitrary‑precision
       type. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < L; ++i) {
        result = result * (seq_t)10 + digits[i];
    }
    return result;
}

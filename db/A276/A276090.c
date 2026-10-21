/*
 * A276090  Left inverse of A276089: For n = sum_{i=1..} d(i)*i! (with each
 *          d(i) <= i), a(n) = sum_{j=1..} d(2j-1)*j!.
 * https://oeis.org/A276090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7815, u16 n <= 7815, u32 n <= 7815, u64 n <= 7815,
 *            u128 n <= 7815, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A276090:  a(n) = sum_{j>=1} d(2j-1) * j!,
   where n = Σ_{i>=1} d(i)*i! is its factorial base representation
   (with 0 ≤ d(i) ≤ i).  The function extracts the odd-position
   factorial-base digits of n and recomposes a number with
   factorial base indices 1,2,3,... . */

/* Requires: seq_t and idx_t types defined by build (e.g. uint64_t) */
/* No external headers needed beyond those supplied by build. */

seq_t a(idx_t n) {
    idx_t cur = n;          /* Remaining value to decompose */
    seq_t ans = 0;           /* Result accumulator */
    seq_t fact_j = 1;        /* Current factorial value j! */
    idx_t i = 1;             /* Factorial-base position (1-indexed) */
    idx_t j = 1;             /* Index for j! (maps odd i to j) */

    while (cur > 0) {
        idx_t base = (idx_t)(i + 1);          /* (i+1) is the modulus */
        idx_t di = cur % base;               /* Digit d(i) */
        cur /= base;                         /* Update remainder */

        if ((i & 1) == 1) {                  /* Odd position */
            ans += (seq_t)di * fact_j;       /* Add d(i) * j! */
            fact_j *= (seq_t)(j + 1);         /* Prepare (j+1)! for next odd i */
            ++j;
        }
        ++i;
    }
    return ans;
}

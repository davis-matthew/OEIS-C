/*
 * A309589  Number subsets {0, ..., 10^k - 1} written in base 10 and sorted
 *          lexicographically, for k = 1, 2, ...
 * https://oeis.org/A309589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, tabf
 * Formula:   a(n) = value at the n‑th position in the concatenation of rows 0
 *            … 10^k−1 for k = 1, 2, …, where each row lists the numbers 0 …
 *            10^k−1 sorted lexicographically. The function finds the row k
 *            containing the index n and then walks the lexicographic tree of
 *            numbers < 10^k to return the desired value
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute 10^k in idx_t, using uint64_t arithmetic to avoid
   overflow for small k (k ≤ 19 for the given n limit). */
static inline idx_t pow10_u64(idx_t exp)
{
    idx_t result = 1;
    while (exp-- > 0)
        result *= 10;
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* ----------------------------------------------------------
       1. Find the row k such that
              sum_{i=1}^{k-1} 10^i ≤ n < sum_{i=1}^{k} 10^i
          and offset = n - sum_{i=1}^{k-1} 10^i.
       ---------------------------------------------------------- */
    idx_t rem = n;           // remaining index to be placed
    idx_t pow10 = 10;        // 10^k for current k
    idx_t k = 1;             // current k

    while (rem >= pow10) {      // skip whole rows
        rem -= pow10;          // subtract this entire row
        k++;
        /* Prevent overflow – for the given n (< 10000) this never
           triggers, but it makes the function robust for larger n. */
        if (pow10 > ((idx_t)(-1)) / 10)
            break;
        pow10 *= 10;
    }
    idx_t offset = rem;        // 0‑based position inside the k‑th row

    /* ----------------------------------------------------------
       2. If offset == 0, the number is 0 in every row.
       ---------------------------------------------------------- */
    if (offset == 0)
        return (seq_t)0;

    /* ----------------------------------------------------------
       3. Generate the lexicographically (offset‑1)-th number
          among 1 … 10^k – 1.  Since 10^k ≤ 10^4 for n ≤ 10000,
          a simple walk suffices.
       ---------------------------------------------------------- */
    idx_t bound = pow10 - 1;           /* maximum number in this row */
    idx_t cur = 1;                     /* start with the first non‑zero */
    idx_t steps = offset - 1;          /* number of steps to skip */

    while (steps-- > 0) {
        if (cur <= bound / 10) {       /* can descend */
            cur *= 10;
        } else {                       /* go to next sibling */
            /* strip trailing 9s to avoid overflow */
            while (cur % 10 == 9 && cur > 0)
                cur /= 10;
            cur += 1;
        }
    }

    return (seq_t)cur;
}

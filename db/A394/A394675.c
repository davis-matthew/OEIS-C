/*
 * A394675  Start with a(1) = 1, a(2) = 2, followed by a stream of 0's. At any
 *          step, the value of a(n-1) is added to the positions from a(n+1) to
 *          a(n+a(n)).
 * https://oeis.org/A394675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2. For n≥1, add a(n) to all positions * from n+2 to
 *            n+1+a(n+1). This implementation expands the * ordinary
 *            range‑addition into two auxiliary arrays to avoid * negative
 *            intermediate values
 * Range:     u8 n <= 17, u16 n <= 28, u32 n <= 51, u64 n <= 97, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)2;

    /* Allocate auxiliary arrays.  Size n+3 is enough for indices
     * up to n+2 (start) and n+1 (end+1).  We use calloc so every
     * element starts at zero.
     */
    size_t sz = (size_t)n + 3;
    seq_t *inc = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *rem = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!inc || !rem) {
        /* Allocation failed – abort with zero (unlikely in tests). */
        return (seq_t)0;
    }

    /* Initial update for n = 1: add a(1) to positions 3..4. */
    inc[3] = (seq_t)1;
    if ((size_t)5 <= n) rem[5] = (seq_t)1;

    seq_t prev = (seq_t)2;          /* a(2) */
    seq_t curAdd = (seq_t)0;

    for (size_t i = 3; i <= n; ++i) {
        /* Apply starts and ends that affect position i. */
        curAdd += inc[i];
        curAdd -= rem[i];

        seq_t ai = curAdd;          /* a(i) */

        /* Schedule update for the previous term (prev = a(i-1)). */
        size_t start = i + 1;
        if (start <= n) inc[start] += prev;

        /* Determine whether an end within the computed range exists. */
        if (i < n) {
            /* end+1 <= n  <=>  ai <= (n - i - 1)  */
            idx_t remaining = (idx_t)n - (idx_t)i - 1;
            if (ai <= (seq_t)remaining) {
                size_t end_plus_one = i + (size_t)ai + 1;
                if (end_plus_one <= n) rem[end_plus_one] += prev;
            }
        }

        prev = ai;
    }

    seq_t result = prev;
    free(inc);
    free(rem);
    return result;
}

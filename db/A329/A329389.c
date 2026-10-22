/*
 * A329389  Solution sequence (a(n)) of the complementary equation a(n) = 6
 *          b(n+1) - b(n), with b(0) = 1.
 * https://oeis.org/A329389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*b(n+1) - b(n), b(0)=1, sequences increasing
 *            complementary
 * Range:     u8 n <= 33, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* check whether value v appears in the first len elements of array a */
static bool a_contains(const seq_t *a, idx_t len, seq_t v)
{
    idx_t lo = 0, hi = len;          /* search interval: [lo, hi) */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (a[mid] == v) return true;
        if (a[mid] < v)  lo = mid + 1;
        else             hi = mid;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)11;            /* a(0) = 11 */

    size_t a_len = (size_t)n + 1;     /* indices 0 .. n */
    size_t b_len = (size_t)n + 2;     /* indices 0 .. n+1 */

    seq_t *a_vals = (seq_t*)malloc(a_len * sizeof(seq_t));
    seq_t *b_vals = (seq_t*)malloc(b_len * sizeof(seq_t));
    if (!a_vals || !b_vals) {
        /* memory allocation failed – return zero */
        if (a_vals) free(a_vals);
        if (b_vals) free(b_vals);
        return (seq_t)0;
    }

    /* initial values */
    a_vals[0] = (seq_t)11;
    b_vals[0] = (seq_t)1;

    /* first derived b value */
    b_vals[1] = (a_vals[0] + b_vals[0]) / (seq_t)6;

    for (idx_t i = 0; i < n; ++i) {
        seq_t candidate = a_vals[i] + (seq_t)1;
        while (1) {
            seq_t sum = candidate + b_vals[i + 1];
            if (sum % (seq_t)6 == (seq_t)0) {
                seq_t next_b = sum / (seq_t)6;

                /* b must be strictly increasing */
                if (next_b <= b_vals[i + 1]) {
                    candidate++;
                    continue;
                }

                /* next_b must not coincide with a previous a value */
                if (!a_contains(a_vals, i + 1, next_b)) {
                    a_vals[i + 1] = candidate;
                    b_vals[i + 2] = next_b;
                    break;
                }
            }
            candidate++;               /* try next candidate */
        }
    }

    seq_t result = a_vals[n];
    free(a_vals);
    free(b_vals);
    return result;
}

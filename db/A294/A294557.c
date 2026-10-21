/*
 * A294557  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) + b(n-2) + n - 1, where a(0) = 1, a(1) = 2, b(0) = 3, and
 *          (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A294557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+b(n-1)+b(n-2)+n-1; Initial values: a(0)=1,
 *            a(1)=2, b(0)=3; The sequences a() and b() are increasing
 *            complementary sequences
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* calculate the n-th term of A294557 */
seq_t a(idx_t n) {
    /* Handle the trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Allocate space for a[0..n] and b[0..n-1] */
    size_t size_a = (size_t)n + 1;
    size_t size_b = (size_t)n;                /* need up to b[n-1] */
    seq_t *a_arr = (seq_t *)calloc(size_a, sizeof(seq_t));
    seq_t *b_arr = (seq_t *)calloc(size_b, sizeof(seq_t));
    if (!a_arr || !b_arr) { /* allocation failed */
        if (a_arr) free(a_arr);
        if (b_arr) free(b_arr);
        return (seq_t)0;                     /* undefined, but avoid crash */
    }

    /* Initial values */
    a_arr[0] = (seq_t)1;
    a_arr[1] = (seq_t)2;
    b_arr[0] = (seq_t)3;

    /* Simple bit‑set for small numbers up to about 3*n+1000.
       Next_missing stays well below this bound (≈ n+5). */
    size_t max_small = 3 * n + 1000;
    bool *used = (bool *)calloc(max_small + 1, sizeof(bool));
    if (!used) {
        free(a_arr);
        free(b_arr);
        return (seq_t)0;
    }
    used[1] = true;
    used[2] = true;
    used[3] = true;
    idx_t next_missing = 4;

    /* Main loop: compute terms up to a[n] */
    for (idx_t i = 2; i <= n; ++i) {
        idx_t b_idx = i - 1;                /* index for b(i-1) */

        /* Compute b(i-1) as the current smallest unused integer */
        seq_t b_val = (seq_t)next_missing;
        b_arr[b_idx] = b_val;

        /* Mark b_val as used and advance next_missing */
        if ((size_t)b_val <= max_small) {
            used[(size_t)b_val] = true;
            while (next_missing <= (idx_t)max_small && used[(size_t)next_missing])
                ++next_missing;
        } else {
            /* b_val > max_small: can happen only for very large n */
            ++next_missing;
        }

        /* Compute a(i) using the recurrence */
        seq_t a_val = a_arr[i - 1] + a_arr[i - 2]
                     + b_arr[b_idx] + b_arr[b_idx - 1]
                     + (seq_t)(i - 1);
        a_arr[i] = a_val;

        /* Mark a_val as used if it falls within our small range,
           and advance next_missing if necessary */
        if ((size_t)a_val <= max_small) {
            used[(size_t)a_val] = true;
            while (next_missing <= (idx_t)max_small && used[(size_t)next_missing])
                ++next_missing;
        }
    }

    /* Final result */
    seq_t result = a_arr[n];

    /* Clean up */
    free(a_arr);
    free(b_arr);
    free(used);

    return result;
}

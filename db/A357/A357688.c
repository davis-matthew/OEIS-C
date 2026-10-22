/*
 * A357688  Number of ways to write n as an ordered sum of four positive
 *          Fibonacci numbers (with a single type of 1).
 * https://oeis.org/A357688
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered quadruples of positive Fibonacci
 *            numbers (with a single type of 1) that sum to n. Equivalent to
 *            the coefficient of x^n in (∑_{k≥2} x^{Fib(k)})^4
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: This implementation recomputes the counts for every call.
   The inner loops have 19^4 = 130321 iterations, which is trivial. */

#include <stdlib.h>
#include <string.h>

static const seq_t fib_vals[19] = {
    1, 2, 3, 5, 8, 13, 21, 34, 55, 89,
    144, 233, 377, 610, 987, 1597, 2584, 4181, 6765
};

seq_t a(idx_t n) {
    // For n < 4 the count is zero (the offset is 4).
    if (n < 4) {
        return 0;
    }

    // Allocate array to hold counts for sums up to n.
    struct seq_alloc_struct {
        seq_t *data;
    } arr;
    arr.data = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!arr.data) {
        return 0; /* allocation failed – return zero as fallback */
    }
    memset(arr.data, 0, (size_t)(n + 1) * sizeof(seq_t));

    const seq_t limit = (seq_t)n;
    const size_t F_LEN = 19;
    idx_t i, j, k, l;
    for (i = 0; i < F_LEN; ++i) {
        for (j = 0; j < F_LEN; ++j) {
            for (k = 0; k < F_LEN; ++k) {
                for (l = 0; l < F_LEN; ++l) {
                    seq_t sum = fib_vals[i] + fib_vals[j] +
                                fib_vals[k] + fib_vals[l];
                    if (sum <= limit) {
                        arr.data[(size_t)sum] += 1;
                    }
                }
            }
        }
    }

    seq_t result = arr.data[n];
    free(arr.data);
    return result;
}

/*
 * A325245  Number of integer partitions of n with adjusted frequency depth 3.
 * https://oeis.org/A325245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d|n} (p_distinct(d) - 1), where p_distinct(m) is the
 *            number of partitions of m into distinct parts (unordered, each
 *            part used at most once)
 * Range:     u8 n <= 29, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *p_distinct = NULL;
static size_t p_curr_max = 0;

/* Compute p_distinct[0..N] using DP: p_distinct[0] = 1. */
static void ensure_partitions(idx_t N) {
    if ((idx_t)p_curr_max >= N) return;   /* already computed */

    size_t new_size = (size_t)N + 1;
    seq_t *new_ptr = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_ptr) exit(1);

    for (size_t i = 0; i < new_size; ++i) new_ptr[i] = 0;
    new_ptr[0] = 1;   /* empty partition */

    for (idx_t k = 1; k <= N; ++k) {
        for (idx_t m = N; m >= k; --m) {
            new_ptr[m] = new_ptr[m] + new_ptr[m - k];
        }
    }

    free(p_distinct);
    p_distinct = new_ptr;
    p_curr_max = (size_t)N;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;

    ensure_partitions(n);           /* p_distinct needed up to n */

    seq_t total = 0;
    for (idx_t i = 1; (i * i) <= n; ++i) {
        if (n % i == 0) {
            seq_t add = p_distinct[i] - 1;
            total += add;

            idx_t j = n / i;
            if (j != i) {
                seq_t add2 = p_distinct[j] - 1;
                total += add2;
            }
        }
    }
    return total;
}

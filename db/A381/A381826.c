/*
 * A381826  G.f. A(x) satisfies A(x) = C(x) / (1 - x*A(x)^2), where C(x) is
 *          the g.f. of A000108.
 * https://oeis.org/A381826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n) + Σ_{i=0}^{n-1} a(i)*b(n-1-i) where b(m) =
 *            Σ_{j=0}^m a(j)*a(m-j). Recurrence derived from A(x) = C(x)/(1 -
 *            x*A(x)^2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"

static seq_t *a_arr = NULL;
static seq_t *b_arr = NULL;
static idx_t arr_cap = 0;
static idx_t next_compute = 0;  /* first index not yet computed */

static void ensure_capacity(idx_t n) {
    if (arr_cap > n) return;
    size_t new_cap = n + 1;
    seq_t *new_a = (seq_t*)malloc(new_cap * sizeof(seq_t));
    seq_t *new_b = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (a_arr) {
        for (idx_t i = 0; i < arr_cap; ++i) {
            new_a[i] = a_arr[i];
            new_b[i] = b_arr[i];
        }
        free(a_arr);
        free(b_arr);
    }
    a_arr = new_a;
    b_arr = new_b;
    arr_cap = new_cap;
}

seq_t a(idx_t n) {
    /* initialise base case a(0)=1, b(0)=1 */
    if (arr_cap == 0) {
        ensure_capacity(0);
        a_arr[0] = 1;
        b_arr[0] = 1;   /* a0*a0 */
        next_compute = 1;
    }

    if (n < next_compute) {
        /* value already computed */
        return a_arr[n];
    }

    ensure_capacity(n);

    for (idx_t k = next_compute; k <= n; ++k) {
        /* compute a_k = Catalan(k) + sum_{i=0}^{k-1} a_i * b_{k-1-i} */
        seq_t sum = 0;
        for (idx_t i = 0; i < k; ++i) {
            sum += a_arr[i] * b_arr[k - 1 - i];
        }
        a_arr[k] = catalan(k) + sum;

        /* compute b_k = sum_{i=0}^{k} a_i * a_{k-i} */
        seq_t sumb = 0;
        for (idx_t i = 0; i <= k; ++i) {
            sumb += a_arr[i] * a_arr[k - i];
        }
        b_arr[k] = sumb;
    }

    next_compute = n + 1;
    return a_arr[n];
}

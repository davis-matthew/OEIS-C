/*
 * A337296  Number whose sum and product of ternary representation digits are
 *          equal.
 * https://oeis.org/A337296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 509, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A337296: numbers whose sum of ternary digits equals their product.
   The terms are listed in increasing order.  The function computes the
   n-th term (a(1)=0, a(2)=1, a(3)=2, …). */

#include <stdint.h>
#include <stdbool.h>

static idx_t binom_u64(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

static seq_t build_term(idx_t L, idx_t k, idx_t rank)
{
    seq_t val = 0;
    idx_t k_rem = k;
    for (idx_t pos = 0; pos < L; ++pos) {
        idx_t rem = L - pos - 1;
        idx_t count1 = 0;
        if (k_rem <= rem) {
            count1 = binom_u64(rem, k_rem);
        }
        if (rank < count1) {
            val = val * 3 + (seq_t)1;
        } else {
            rank -= count1;
            k_rem -= 1;
            val = val * 3 + (seq_t)2;
        }
    }
    return val;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;

    idx_t pos = n - 2;   /* 0‑based index among positive terms */

    for (idx_t k = 0; ; ++k) {
        idx_t L;
        idx_t block;

        if (k == 0) {
            L = 1;
            block = 1;
        } else if (k == 1) {
            L = 1;
            block = 1;
        } else if (k == 2) {
            L = 2;
            block = 1;
        } else {
            L = ((idx_t)1 << k) - k;
            block = binom_u64(L, k);
        }

        if (pos < block) {
            if (k == 0) return (seq_t)1;
            if (k == 1) return (seq_t)2;
            if (k == 2) return (seq_t)8;
            return build_term(L, k, pos);
        }
        pos -= block;
    }   /* never reached */
}

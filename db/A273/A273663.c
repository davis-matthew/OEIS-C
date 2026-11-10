/*
 * A273663  Least monotonic left inverse for A273670: a(1) = 0; for n > 1,
 *          a(n) = A257680(A225901(n)) + a(n-1).
 * https://oeis.org/A273663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; a(n) = A257680(A225901(n)) + a(n-1). A225901 maps each
 *            nonzero factorial-base digit d at position k to k+1-d, so the
 *            new digit is 1 exactly when d = k (a maximal digit). Hence
 *            A257680(A225901(n)) = 1 iff n has a maximal factorial-base
 *            digit, and a(n) counts such k in 2..n
 * Range:     u8 n <= 327, u16 n <= 10080, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 100 terms through n = 9857 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static bool A273663_has_max_digit(idx_t n)
{
    idx_t k = 1;
    while (n > 0) {
        idx_t d = n % (k + 1);
        if (d == k) return true;
        n /= (k + 1);
        k++;
    }
    return false;
}

static seq_t *A273663_cache = NULL;
static idx_t A273663_cnt = 0;   /* cache[i] = a(i), valid for i < cnt */
static idx_t A273663_cap = 0;

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    if (!A273663_cache) {
        A273663_cap = 1024;
        A273663_cache = mem_new(seq_t, (size_t)A273663_cap);
        A273663_cache[0] = 0;
        A273663_cache[1] = 0;
        A273663_cnt = 2;
    }
    if (n >= A273663_cnt) {
        if (n >= A273663_cap) {
            idx_t nc = A273663_cap;
            while (nc <= n) nc *= 2;
            A273663_cache = mem_resize(seq_t, A273663_cache, (size_t)nc);
            A273663_cap = nc;
        }
        for (idx_t i = A273663_cnt; i <= n; i++) {
            seq_t v = A273663_cache[i - 1];
            if (A273663_has_max_digit(i)) v = v + 1;
            A273663_cache[i] = v;
        }
        A273663_cnt = n + 1;
    }
    return A273663_cache[n];
}

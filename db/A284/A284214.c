/*
 * A284214  Remainder when sum of first n terms of A006949 is divided by n.
 * https://oeis.org/A284214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Sum_{k=0..n-1} A006949(k)) mod n, where A006949(n) =
 *            A006949(n-1-A006949(n-1)) + A006949(n-2-A006949(n-2)),
 *            A006949(0)=A006949(1)=A006949(2)=1
 * Range:     u8 n <= 263, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 96 terms through n = 9601 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A006949_a(idx_t n) {
    static seq_t *memo = NULL;
    static idx_t memo_cap = 0;
    static idx_t computed_up_to = 0;

    if (memo == NULL) {
        memo_cap = 64;
        memo = mem_new(seq_t, memo_cap);
        memo[0] = 1;
        memo[1] = 1;
        memo[2] = 1;
        computed_up_to = 2;
    }

    if (n <= computed_up_to) {
        return memo[n];
    }

    while (n >= memo_cap) {
        memo_cap *= 2;
        memo = mem_resize(seq_t, memo, memo_cap);
    }

    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        idx_t idx1 = i - 1 - (idx_t)memo[i - 1];
        idx_t idx2 = i - 2 - (idx_t)memo[i - 2];
        memo[i] = memo[idx1] + memo[idx2];
    }

    computed_up_to = n;
    return memo[n];
}

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    seq_t sum = 0;
    (void)A006949_a(n);
    for (idx_t k = 0; k < n; k++) {
        sum = (sum + A006949_a(k) % m) % m;
    }
    return sum;
}

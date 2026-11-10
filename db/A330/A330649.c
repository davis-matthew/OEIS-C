/*
 * A330649  Expansion of e.g.f. Product_{k>=1} 1 / (1 - x^k/(k!*(1 - x)^k)).
 * https://oeis.org/A330649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n-1,k-1) * A005651(k) * n!/k!, with
 *            a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 199 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static struct {
    seq_t **binom_table;
    seq_t *part_memo;
    idx_t n;
    int built;
} A005651_state = {NULL, NULL, 0, 0};

static seq_t A005651_binom(idx_t n, idx_t k) {
    if (k > n) return 0;
    return A005651_state.binom_table[n][k];
}

static seq_t A005651_part(idx_t remaining, idx_t max_part) {
    if (remaining == 0) return 1;
    idx_t index = remaining * (A005651_state.n + 1) + max_part;
    if (A005651_state.part_memo[index] != 0) return A005651_state.part_memo[index];
    seq_t result = 0;
    for (idx_t part = (remaining < max_part) ? remaining : max_part; part >= 1; part--) {
        result += A005651_binom(remaining, part) * A005651_part(remaining - part, part);
    }
    A005651_state.part_memo[index] = result;
    return result;
}

static void A005651_ensure(idx_t n) {
    if (A005651_state.built && A005651_state.n >= n) return;
    if (A005651_state.built) {
        for (idx_t i = 0; i <= A005651_state.n; i++) mem_free(A005651_state.binom_table[i]);
        free(A005651_state.binom_table);
        mem_free(A005651_state.part_memo);
    }
    A005651_state.binom_table = (seq_t **)malloc((n + 1) * sizeof(seq_t *));
    for (idx_t i = 0; i <= n; i++) {
        A005651_state.binom_table[i] = mem_new(seq_t, i + 1);
        A005651_state.binom_table[i][0] = 1;
        if (i > 0) A005651_state.binom_table[i][i] = 1;
        for (idx_t j = 1; j < i; j++)
            A005651_state.binom_table[i][j] = A005651_state.binom_table[i-1][j-1] + A005651_state.binom_table[i-1][j];
    }
    A005651_state.part_memo = mem_new(seq_t, (n + 1) * (n + 2));
    A005651_state.n = n;
    A005651_state.built = 1;
}

static seq_t A005651_a(idx_t n) {
    if (n == 0) return 1;
    A005651_ensure(n);
    return A005651_part(n, n);
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    A005651_ensure(n);
    seq_t *row = mem_new(seq_t, n + 1);
    seq_t *fp = mem_new(seq_t, n + 2);
    row[0] = 1;
    for (idx_t i = 1; i + 1 <= n; i++) {
        for (idx_t j = i; j >= 1; j--) row[j] += row[j - 1];
    }
    fp[n] = 1;
    for (idx_t k = n; k >= 1; k--) {
        fp[k - 1] = fp[k] * (seq_t)k;
    }
    /* fp[k] currently holds n!/k! shifted: recompute properly */
    fp[n] = 1;
    for (idx_t k = n; k >= 1; k--) {
        if (k == n) fp[k] = 1;
        if (k >= 1) fp[k - 1] = fp[k] * (seq_t)k;
    }
    seq_t total = 0;
    for (idx_t k = 1; k <= n; k++) {
        total += row[k - 1] * A005651_a(k) * fp[k];
    }
    mem_free(row);
    mem_free(fp);
    return total;
}

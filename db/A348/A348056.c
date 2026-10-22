/*
 * A348056  Numbers k where the d(j)-th digit is j for d(j) and j > 0 and d(j)
 *          = 0 if and only if j is not a digit of k.
 * https://oeis.org/A348056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   seqcmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seqcmp.h"

/* Sequence: Numbers k where the d(j)-th digit is j for j>0 and
 * d(j) = 0 iff j is not a digit of k.
 *
 * Implementation: enumerate all involutions of the set {1,…,N}
 * for N = 1 … 9, then for each involution zero any subset of its
 * fixed points (except a leading zero). Collect all numbers, sort
 * and deduplicate. The sequence starts with 0 for N = 0.
 */
static seq_t *global_seq = NULL;
static size_t global_len = 0;
static size_t global_cap = 0;

/* append a value to the global array, growing capacity as needed */
static void push_seq(seq_t val) {
    if (global_seq == NULL) {
        global_cap = 1024;
        global_seq = (seq_t *)malloc(global_cap * sizeof(seq_t));
        if (!global_seq) exit(1);
    } else if (global_len == global_cap) {
        global_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(global_seq, global_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        global_seq = tmp;
    }
    global_seq[global_len++] = val;
}

/* process a complete mapping: generate all numbers by zeroing
 * optional fixed points (except a leading zero). */
static void process_mapping(int *mapping, int fixed_count, int *fixed_pos, int N) {
    int fixed_index[9];
    for (int i = 0; i < N; ++i) fixed_index[i] = -1;
    for (int k = 0; k < fixed_count; ++k) fixed_index[fixed_pos[k]] = k;

    int masks = 1 << fixed_count;
    for (int mask = 0; mask < masks; ++mask) {
        /* skip a leading zero */
        if (fixed_index[0] >= 0 && (mask & (1 << fixed_index[0]))) continue;

        seq_t val = 0;
        for (int i = 0; i < N; ++i) {
            int d = mapping[i];
            if (fixed_index[i] >= 0 && (mask & (1 << fixed_index[i]))) d = 0;
            val = val * 10 + d;
        }
        push_seq(val);
    }
}

/* recursive generation of all involutions of {1..N} */
static void gen_inv_helper(int N, int *mapping, char *used,
                           int fixed_count, int *fixed_pos) {
    int i;
    for (i = 0; i < N; ++i) if (!used[i]) break;
    if (i == N) {
        process_mapping(mapping, fixed_count, fixed_pos, N);
        return;
    }

    /* leave i fixed */
    used[i] = 1;
    mapping[i] = i + 1;
    fixed_pos[fixed_count] = i;
    gen_inv_helper(N, mapping, used, fixed_count + 1, fixed_pos);
    used[i] = 0;

    /* pair i with j */
    for (int j = i + 1; j < N; ++j) if (!used[j]) {
        used[i] = used[j] = 1;
        mapping[i] = j + 1;
        mapping[j] = i + 1;
        gen_inv_helper(N, mapping, used, fixed_count, fixed_pos);
        used[i] = used[j] = 0;
    }
}

/* generate all numbers for a fixed number of digits N */
static void generate_for_n(int N) {
    int mapping[9];
    char used[9];
    int fixed_pos[9];
    for (int i = 0; i < N; ++i) used[i] = 0;
    gen_inv_helper(N, mapping, used, 0, fixed_pos);
}

/* one‑time initialization of the entire sequence */
static void init_sequence(void) {
    /* N = 0 term */
    push_seq((seq_t)0);

    for (int N = 1; N <= 9; ++N) generate_for_n(N);

    qsort(global_seq, global_len, sizeof(seq_t), seqcmp);

    /* deduplicate (duplicates are extremely unlikely but safe) */
    size_t write = 0;
    for (size_t i = 0; i < global_len; ++i) {
        if (write == 0 || global_seq[write - 1] != global_seq[i]) {
            global_seq[write++] = global_seq[i];
        }
    }
    global_len = write;
}

/* public sequence function */
seq_t a(idx_t n) {
    static int initialized = 0;
    if (!initialized) {
        init_sequence();
        initialized = 1;
    }
    return global_seq[(size_t)(n - 1)];
}

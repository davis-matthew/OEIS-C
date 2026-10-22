/*
 * A338919  Square array of distinct positive integers T(n, k), n > 0, k > 0,
 *          read by antidiagonals upwards, filled the greedy way such that for
 *          any n > 0 and k > 0, T(n, k) is divisible by n and by k.
 * https://oeis.org/A338919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabl
 * Formula:   * Compute the OEIS sequence A338919: square array T(n, k) filled
 *            greedily * with the smallest unused integer divisible by both n
 *            and k, read by * antidiagonals upward. The function returns the
 *            n‑th element of that * sequence (1‑based index)
 * Range:     u8 n <= 74, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Maximum row/column needed for n up to 10000 (diagonal count ≈ 141) */
#define MAX_ORDER 200

static seq_t T_arr[MAX_ORDER + 1][MAX_ORDER + 1];
static uint8_t T_set[MAX_ORDER + 1][MAX_ORDER + 1] = {0};

/* Used values stored as 64‑bit integers for fast membership checks.
 * The sequence values themselves are cast to seq_t upon return. */
static uint64_t *used_vals = NULL;
static size_t used_cnt = 0;
static size_t used_cap = 0;

/* Ensure capacity of the used values array. */
static void ensure_used_cap(void) {
    if (used_cap == 0) {
        used_cap = 1024;
        used_vals = (uint64_t *)malloc(used_cap * sizeof(uint64_t));
        if (!used_vals) exit(1); /* allocation failure */
    } else if (used_cnt == used_cap) {
        used_cap *= 2;
        uint64_t *tmp = (uint64_t *)realloc(used_vals, used_cap * sizeof(uint64_t));
        if (!tmp) exit(1);
        used_vals = tmp;
    }
}

/* Linear search for a value in the used list. */
static bool used_contains(uint64_t v) {
    for (size_t i = 0; i < used_cnt; ++i)
        if (used_vals[i] == v) return true;
    return false;
}

/* Add a value to the used list. */
static void mark_used(uint64_t v) {
    ensure_used_cap();
    used_vals[used_cnt++] = v;
}

/* Retrieve (and memoise) T(n,k).  n and k are 1‑based and <= MAX_ORDER. */
static seq_t get_T(idx_t n, idx_t k) {
    size_t i = (size_t)n;
    size_t j = (size_t)k;
    if (T_set[i][j]) return T_arr[i][j];

    /* Compute lcm(n,k) using 64‑bit arithmetic to avoid overflow. */
    seq_t g = gcd((seq_t)n, (seq_t)k);
    uint64_t l = ((uint64_t)n / (uint64_t)g) * (uint64_t)k;

    uint64_t cand = l;
    while (used_contains(cand)) {
        cand += l;
    }

    mark_used(cand);

    T_set[i][j] = 1;
    T_arr[i][j] = (seq_t)cand;
    return T_arr[i][j];
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, return 0 as placeholder */
    idx_t target = n;
    idx_t count = 0;

    /* Iterate over antidiagonals: sum = s = n + k */
    for (idx_t s = 2; ; ++s) {
        for (idx_t k = 1; k < s; ++k) {
            idx_t row = s - k;
            seq_t val = get_T(row, k);
            ++count;
            if (count == target) {
                return val; /* cast to seq_t automatically */
            }
        }
    }
}

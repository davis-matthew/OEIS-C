/*
 * A321512  Characteristic function of the reverse in the shuffle (perfect
 *          faro shuffle with cut): 1 if the sequence of shuffles of n cards
 *          contains the reverse of the original order of cards, 0 if not.
 * https://oeis.org/A321512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the reverse order of n cards appears in the orbit of
 *            the perfect faro shuffle with cut (as defined in the problem),
 *            otherwise 0
 * Range:     u8 n <= 9213, u16 n <= 9213, u32 n <= 9213, u64 n <= 9213,
 *            u128 n <= 9213, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* gcd for idx_t (which is unsigned, e.g., uint64_t) */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* defn for n ≥ 1 only */

    idx_t half = n / 2;

    /* allocate arrays; cast malloc return for C++ */
    idx_t *perm      = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    idx_t *visited   = (idx_t *)calloc((size_t)n, sizeof(idx_t));
    idx_t *cycle_buf = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!perm || !visited || !cycle_buf) {
        free(perm); free(visited); free(cycle_buf);
        return 0;
    }

    /* build permutation:   new position for each old position (0‑based) */
    for (idx_t i = 0; i < n; ++i) {
        idx_t pos = i + 1;                       /* 1‑based index */
        if (pos & 1) {                          /* odd */
            perm[i] = half + (pos + 1) / 2 - 1; /* 0‑based */
        } else {                                 /* even */
            perm[i] = pos / 2 - 1;              /* 0‑based */
        }
    }

    /* arrays to store cycle data */
    idx_t *shift_arr = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    idx_t *len_arr   = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!shift_arr || !len_arr) {
        free(perm); free(visited); free(cycle_buf);
        free(shift_arr); free(len_arr);
        return 0;
    }

    idx_t cycle_cnt = 0;

    /* process each cycle of the permutation */
    for (idx_t i = 0; i < n; ++i) {
        if (visited[i]) continue;

        /* collect the cycle */
        idx_t cur_len = 0;
        for (idx_t v = i; !visited[v]; v = perm[v]) {
            visited[v] = 1;
            cycle_buf[cur_len++] = v;
        }

        /* find shift that aligns this cycle with the reverse permutation */
        idx_t target = n - 1 - cycle_buf[0];     /* element that should be
                                                   the image of cycle_buf[0]
                                                   under the reverse permutation */
        idx_t shift  = cur_len;                 /* sentinel: not found yet */
        for (idx_t p = 0; p < cur_len; ++p) {
            if (cycle_buf[p] == target) {
                shift = p;
                break;
            }
        }

        /* if reverse image not in the same cycle, impossible */
        if (shift == cur_len) {
            free(perm); free(visited); free(cycle_buf);
            free(shift_arr); free(len_arr);
            return 0;
        }

        /* verify that the same shift works for all elements of the cycle */
        bool ok = true;
        for (idx_t j = 0; j < cur_len; ++j) {
            idx_t r = n - 1 - cycle_buf[j];
            idx_t idx = (j + shift) % cur_len;
            if (r != cycle_buf[idx]) { ok = false; break; }
        }
        if (!ok) {
            free(perm); free(visited); free(cycle_buf);
            free(shift_arr); free(len_arr);
            return 0;
        }

        /* store cycle data */
        shift_arr[cycle_cnt] = shift;
        len_arr[cycle_cnt]   = cur_len;
        ++cycle_cnt;
    }

    /* cross‑cycle consistency: solve the CRT conditions for the shifts */
    for (idx_t a = 0; a < cycle_cnt; ++a) {
        for (idx_t b = a + 1; b < cycle_cnt; ++b) {
            idx_t g = gcd_idx(len_arr[a], len_arr[b]);
            idx_t diff = (shift_arr[a] >= shift_arr[b])
                         ? shift_arr[a] - shift_arr[b]
                         : shift_arr[b] - shift_arr[a];
            if (diff % g != 0) {
                free(perm); free(visited); free(cycle_buf);
                free(shift_arr); free(len_arr);
                return 0;
            }
        }
    }

    free(perm); free(visited); free(cycle_buf);
    free(shift_arr); free(len_arr);
    return 1;
}

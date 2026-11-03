/*
 * A242437  Numbers not appearing in the sequence of integers, beginning with
 *          1, that can be formed by adding any digit of any previous term to
 *          that previous term.
 * https://oeis.org/A242437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A242437: numbers not appearing in the sequence obtainable from 1 by
   repeatedly adding any decimal digit of the current term.
   The function finds the nth missing number by always recomputing the
   reachable set up to a limit that expands until at least n missing
   numbers are found. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static void recompute_missing(idx_t limit, uint8_t **seen, size_t *seen_len,
                              idx_t **missing, size_t *missing_len) {
    /* Allocate / reallocate the seen bitmap */
    free(*seen);
    *seen = (uint8_t*)malloc((size_t)(limit + 1));
    if (!*seen) return;
    memset(*seen, 0, (size_t)(limit + 1));

    /* BFS queue */
    idx_t *queue = (idx_t*)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!queue) {
        free(*seen);
        *seen = NULL;
        return;
    }
    size_t q_head = 0, q_tail = 0;

    (*seen)[1] = 1;
    queue[q_tail++] = 1;

    while (q_head < q_tail) {
        idx_t t = queue[q_head++];
        idx_t tmp = t;
        while (tmp > 0) {
            int d = (int)(tmp % 10ULL);
            tmp /= 10ULL;
            if (d == 0) continue;
            idx_t u = t + (idx_t)d;
            if (u <= limit && !(*seen)[(size_t)u]) {
                (*seen)[(size_t)u] = 1;
                queue[q_tail++] = u;
            }
        }
    }
    free(queue);

    /* Count missing numbers */
    size_t mlen = 0;
    for (idx_t i = 1; i <= limit; ++i)
        if (!(*seen)[(size_t)i]) ++mlen;

    /* Reallocate missing indices array */
    free(*missing);
    if (mlen == 0) {
        *missing = NULL;
    } else {
        *missing = (idx_t*)malloc(mlen * sizeof(idx_t));
        if (!*missing) {
            free(*seen);
            *seen = NULL;
            *missing_len = 0;
            return;
        }
        size_t pos = 0;
        for (idx_t i = 1; i <= limit; ++i)
            if (!(*seen)[(size_t)i]) (*missing)[pos++] = i;
        *missing_len = mlen;
    }

    *seen_len   = (size_t)(limit + 1);
}

static uint8_t *global_seen   = NULL;
static size_t  global_seen_len = 0;
static idx_t  *global_missing = NULL;
static size_t  global_missing_len = 0;
static idx_t   current_limit    = 0;

static void ensure_missing(idx_t n) {
    while (global_missing_len < n) {
        idx_t new_limit = current_limit ? current_limit * 2 : 4;
        current_limit = new_limit;
        recompute_missing(current_limit, &global_seen, &global_seen_len,
                          &global_missing, &global_missing_len);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for offset 1, but return 0 */
    ensure_missing(n);             /* guarantees global_missing_len >= n */
    return (seq_t)global_missing[n - 1];
}

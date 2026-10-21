/*
 * A281484  Number of ways to write a nonnegative rational integer n as a sum
 *          of three squares in the ring of integers of Q(sqrt 7).
 * https://oeis.org/A281484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of solutions to 
 *   n = x^2 + y^2 + z^2 with x,y,z ∈ Z[√7].
 *
 * The algorithm enumerates all pairs (a,d) with a^2+7d^2 ≤ n,
 * stores for each value of u = a^2+7d^2 the product t = a*d
 * (including multiplicities), then counts all ordered triples of
 * such pairs whose u-sum equals n and whose t-sum is zero.
 */

#include <stdint.h>
#include <stdlib.h>

/* helper: floor(sqrt(v)) for non‑negative v, works for 64‑bit values */
static inline idx_t int_sqrt(idx_t v) {
    idx_t low = 0, high = v, mid, res = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        if (mid > 0 && mid > v / mid) {
            high = mid - 1;
        } else {
            res = mid;
            low = mid + 1;
        }
    }
    return res;
}

typedef struct Entry {
    int t;
    uint32_t count;
    struct Entry *next;
} Entry;

seq_t a(idx_t n) {
    /* allocate list heads for each possible u = a^2+7d^2 */
    size_t N = (size_t)n + 1;
    Entry **lists = (Entry **)calloc(N, sizeof(Entry *));
    if (!lists) return 0;  /* allocation failure */

    /* generate all pairs (a,d) */
    int max_a = (int)int_sqrt((uint64_t)n);
    for (int a = -max_a; a <= max_a; ++a) {
        uint64_t a2 = (uint64_t)a * (uint64_t)a;
        if (a2 > (uint64_t)n) continue;
        uint64_t rem = (uint64_t)n - a2;
        int max_d = (int)int_sqrt(rem / 7ULL);
        for (int d = -max_d; d <= max_d; ++d) {
            uint64_t d2 = (uint64_t)d * (uint64_t)d;
            uint64_t u = a2 + 7ULL * d2;   /* a^2 + 7d^2 */
            int t = a * d;                 /* a*d */
            /* insert or increment in the linked list for this u */
            Entry *e = lists[(size_t)u];
            while (e && e->t != t) e = e->next;
            if (e) {
                e->count++;
            } else {
                e = (Entry *)malloc(sizeof(Entry));
                if (!e) { free(lists); return 0; }
                e->t = t;
                e->count = 1;
                e->next = lists[(size_t)u];
                lists[(size_t)u] = e;
            }
        }
    }

    /* collect all u values that have at least one entry */
    size_t *us = (size_t *)malloc(N * sizeof(size_t));
    size_t ucnt = 0;
    for (size_t u = 0; u < N; ++u) {
        if (lists[u]) us[ucnt++] = u;
    }

    /* count ordered triples */
    seq_t ans = 0;
    for (size_t ii = 0; ii < ucnt; ++ii) {
        size_t u1 = us[ii];
        Entry *e1 = lists[u1];
        while (e1) {
            int t1 = e1->t;
            uint32_t c1 = e1->count;
            for (size_t jj = 0; jj < ucnt; ++jj) {
                size_t u2 = us[jj];
                if (u1 + u2 > n) continue;
                Entry *e2 = lists[u2];
                while (e2) {
                    int t2 = e2->t;
                    uint32_t c2 = e2->count;
                    size_t u3 = (size_t)n - u1 - u2;
                    if (u3 >= N) { e2 = e2->next; continue; }
                    Entry *e3 = lists[u3];
                    int need_t3 = -(t1 + t2);
                    while (e3) {
                        if (e3->t == need_t3) {
                            uint32_t c3 = e3->count;
                            ans += (seq_t)c1 * (seq_t)c2 * (seq_t)c3;
                            break;
                        }
                        e3 = e3->next;
                    }
                    e2 = e2->next;
                }
            }
            e1 = e1->next;
        }
    }

    /* free memory */
    for (size_t u = 0; u < N; ++u) {
        Entry *e = lists[u];
        while (e) {
            Entry *next = e->next;
            free(e);
            e = next;
        }
    }
    free(lists);
    free(us);
    return ans;
}

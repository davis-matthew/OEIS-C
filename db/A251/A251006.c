/*
 * A251006  Number of (n+1)X(4+1) 0..2 arrays with no 2X2 subblock having its
 *          maximum diagonal element less than the absolute difference of its
 *          antidiagonal elements.
 * https://oeis.org/A251006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A251006 – number of (n+1)×5 0..2 arrays with no 2×2 subblock
    whose maximum diagonal element is less than the absolute difference
    of its antidiagonal elements.
    This implementation uses transfer‑matrix dynamic programming over
    the 3⁵ = 243 possible row states.  Each state is an array of five
    digits in base 3.  The graph contains an edge from state s to state t
    if every 2×2 block formed by s followed by t satisfies
    max(diagonal) ≥ |antidiagonal difference|.  DP iterates n steps
    (n = number of 2×2 blocks, i.e. the number of rows − 1) and sums
    the number of paths to all states.  Complexity: O(n·E) with
    E ≈ 5 k² (≈ 3×10⁴).  Works for any seq_t defined by the build
    (uint8_t … unsigned __int128 or arbitrary‑precision). */

#include <stdlib.h>

struct Graph {
    int offsets[244];   /* offsets into edges array, offsets[0] = 0   */
    int *edges;         /* concatenated adjacency lists            */
    seq_t *dp1;         /* DP buffer at current step               */
    seq_t *dp2;         /* DP buffer at next step                  */
};

static struct Graph *graph = NULL;

/* ------------------------------------------------------------------ */
/* Helper: compute the 5 base‑3 digits of a state (0 ≤ state < 243).    */
/* ------------------------------------------------------------------ */
static void get_digits(int state, unsigned char digits[5]) {
    int t = state;
    for (int k = 0; k < 5; ++k) {
        digits[k] = t % 3;
        t /= 3;
    }
}

/* ------------------------------------------------------------------ */
/* Build the transition graph and allocate DP buffers.                 */
/* ------------------------------------------------------------------ */
static void build_graph(void) {
    if (graph != NULL) return;        /* already built */

    /* allocate graph struct */
    graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->dp1 = (seq_t *)calloc(243, sizeof(seq_t));
    graph->dp2 = (seq_t *)calloc(243, sizeof(seq_t));

    /* temporary arrays for edge counts */
    int edge_count[243] = {0};
    unsigned char digits_s[5], digits_t[5];

    /* first pass: count edges per source state */
    for (int s = 0; s < 243; ++s) {
        get_digits(s, digits_s);
        int cnt = 0;
        for (int t = 0; t < 243; ++t) {
            get_digits(t, digits_t);
            int ok = 1;
            for (int c = 0; c < 4; ++c) {
                unsigned char a = digits_s[c];
                unsigned char b = digits_s[c + 1];
                unsigned char c0 = digits_t[c];
                unsigned char d = digits_t[c + 1];
                unsigned char mx = (a > d) ? a : d;
                int diff = (int)b - (int)c0;
                if (diff < 0) diff = -diff;
                if (mx < diff) {    /* condition violated -> edge rejected */
                    ok = 0;
                    break;
                }
            }
            if (ok) cnt++;
        }
        edge_count[s] = cnt;
    }

    /* compute offsets and total number of edges */
    int total_edges = 0;
    graph->offsets[0] = 0;
    for (int s = 0; s < 243; ++s) {
        total_edges += edge_count[s];
        graph->offsets[s + 1] = total_edges;
    }

    /* allocate edges array */
    int *edges_arr = (int *)malloc(total_edges * sizeof(int));
    graph->edges = edges_arr;

    /* second pass: fill edges array */
    int idx = 0;
    for (int s = 0; s < 243; ++s) {
        get_digits(s, digits_s);
        for (int t = 0; t < 243; ++t) {
            get_digits(t, digits_t);
            int ok = 1;
            for (int c = 0; c < 4; ++c) {
                unsigned char a = digits_s[c];
                unsigned char b = digits_s[c + 1];
                unsigned char c0 = digits_t[c];
                unsigned char d = digits_t[c + 1];
                unsigned char mx = (a > d) ? a : d;
                int diff = (int)b - (int)c0;
                if (diff < 0) diff = -diff;
                if (mx < diff) {
                    ok = 0;
                    break;
                }
            }
            if (ok) {
                edges_arr[idx++] = t;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Main function required by the harness.                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* a(1) is defined for n >= 1 only */

    build_graph();                 /* ensure graph is ready */

    seq_t *dp_cur = graph->dp1;
    seq_t *dp_next = graph->dp2;

    /* initialise: exactly one way to start with any row */
    for (int i = 0; i < 243; ++i) dp_cur[i] = 1;

    /* iterate over the n rows‑1 transitions */
    for (idx_t step = 0; step < n; ++step) {
        /* zero next buffer */
        for (int i = 0; i < 243; ++i) dp_next[i] = 0;

        /* propagate counts along edges */
        for (int s = 0; s < 243; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;
            int start = graph->offsets[s];
            int end   = graph->offsets[s + 1];
            for (int e = start; e < end; ++e) {
                int t = graph->edges[e];
                dp_next[t] += val;
            }
        }

        /* swap buffers */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* sum over all ending states */
    seq_t total = 0;
    for (int i = 0; i < 243; ++i) total += dp_cur[i];

    return total;
}

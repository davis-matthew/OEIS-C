/*
 * A248066  Number of length n+5 0..6 arrays with some disjoint triples in
 *          each consecutive six terms having the same sum.
 * https://oeis.org/A248066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+5 arrays with 0..6 entries in which
 *            every consecutive six terms contain two disjoint triples having
 *            equal sum. The computation uses a DFA over the last five entries
 *            and dynamic programming. The state space is 7^5 = 16807.
 *            Transition tables are pre‑computed once
 * Range:     u8 none, u16 n <= 1, u32 n <= 11, u64 n <= 30, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Size constants for the state automaton */
#define BASE   7U
#define POW5   2401U            /* 7^4, used to shift out the oldest digit */
#define STATES 16807U           /* 7^5 */

/* Pre‑computed transition data */
static uint16_t trans_states[STATES][BASE];
static unsigned char trans_allowed[STATES][BASE];
static int trans_inited = 0;

/* Helper: compute all 20 combinations of 3 indices from 0..5 */
static const unsigned char triple_idx[20][3] = {
    {0,1,2},{0,1,3},{0,1,4},{0,1,5},
    {0,2,3},{0,2,4},{0,2,5},
    {0,3,4},{0,3,5},{0,4,5},
    {1,2,3},{1,2,4},{1,2,5},
    {1,3,4},{1,3,5},{1,4,5},
    {2,3,4},{2,3,5},{2,4,5},
    {3,4,5}
};

/* Initialize transition tables once */
static void init_transitions(void) {
    if (trans_inited) return;
    for (size_t s = 0; s < STATES; ++s) {
        /* Decode the 5 digits of the state (base 7) */
        unsigned char d[5];
        size_t t = s;
        for (int i = 4; i >= 0; --i) {
            d[i] = t % BASE;
            t /= BASE;
        }
        for (unsigned char v = 0; v < BASE; ++v) {
            /* Form the 6‑tuple window */
            unsigned char w[6];
            w[0] = d[0]; w[1] = d[1]; w[2] = d[2];
            w[3] = d[3]; w[4] = d[4]; w[5] = v;

            /* Total sum and half sum */
            unsigned int total = 0;
            for (int i = 0; i < 6; ++i) total += w[i];
            if ((total & 1) != 0) {          /* total odd -> impossible */
                trans_allowed[s][v] = 0;
                continue;
            }
            unsigned int half = total >> 1;
            /* Check if some triple sums to half */
            unsigned char ok = 0;
            for (int k = 0; k < 20; ++k) {
                unsigned int sum = w[triple_idx[k][0]] +
                                   w[triple_idx[k][1]] +
                                   w[triple_idx[k][2]];
                if (sum == half) { ok = 1; break; }
            }
            trans_allowed[s][v] = ok;
            /* Compute next state: shift left and add v */
            uint16_t newstate = (uint16_t)((s % POW5) * BASE + v);
            trans_states[s][v] = newstate;
        }
    }
    trans_inited = 1;
}

/* Power of 7 using seq_t arithmetic */
static seq_t pow7_uint16(unsigned n) {
    seq_t r = 1;
    for (unsigned i = 0; i < n; ++i) r *= (seq_t)7;
    return r;
}

seq_t a(idx_t n) {
    /* Convert n to sequence length N = n+5 */
    idx_t N = n + 5;
    /* For very small N (≤5), no window constraints apply */
    if (N <= 5) return pow7_uint16((unsigned)N);

    /* Ensure transition tables are ready */
    init_transitions();

    /* Allocate DP arrays */
    seq_t *cur = (seq_t*)calloc(STATES, sizeof(seq_t));
    seq_t *nxt = (seq_t*)calloc(STATES, sizeof(seq_t));
    if (!cur || !nxt) return 0;                 /* allocation failure */

    /* Initialize DP for the first 5 positions: all 5‑tuples are allowed */
    for (size_t s = 0; s < STATES; ++s) cur[s] = 1;

    /* Process positions 6..N */
    for (idx_t pos = 6; pos <= N; ++pos) {
        /* Clear next */
        for (size_t s = 0; s < STATES; ++s) nxt[s] = 0;
        /* Transition */
        for (size_t s = 0; s < STATES; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;            /* skip zero counts */
            for (unsigned v = 0; v < BASE; ++v) {
                if (!trans_allowed[s][v]) continue;
                uint16_t ns = trans_states[s][v];
                nxt[ns] += val;
            }
        }
        /* Swap */
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
    }

    /* Sum over all final states to get the answer */
    seq_t ans = 0;
    for (size_t s = 0; s < STATES; ++s) ans += cur[s];

    /* Free DP arrays */
    free(cur);
    free(nxt);

    return ans;
}

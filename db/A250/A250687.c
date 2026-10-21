/*
 * A250687  Number of (n+1)X(4+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 25, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A250687.
 *  Recurrence (n > 17):
 *  a(n) = 27*a(n-1) - 331*a(n-2) + 2439*a(n-3) - 12049*a(n-4)
 *         + 42129*a(n-5) - 107225*a(n-6) + 201069*a(n-7) - 277706*a(n-8)
 *         + 278928*a(n-9) - 197984*a(n-10) + 94032*a(n-11)
 *         - 26784*a(n-12) + 3456*a(n-13)
 *
 *  The first 17 terms are computed by dynamic programming on the
 *  1024 possible rows of a 5‑cell array with alphabet {0,1,2,3}.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/* External integer types supplied by the build system. */
/* seq_t may be an unsigned integer type or an arbitrary‑precision class. */
/* idx_t is an unsigned integer type. */

static const int64_t coeffs[13] = {
    27LL,   -331LL,   2439LL,  -12049LL,  42129LL,  -107225LL,
    201069LL, -277706LL, 278928LL, -197984LL,  94032LL,  -26784LL, 3456LL
};

#define ROWS 1024

/* Adjacency list: adjacency[v][k] is the k-th neighbor of row v. */
static uint16_t adjacency[ROWS][ROWS];
static uint16_t adj_len[ROWS];

/* First 17 terms a(1)..a(17). */
static seq_t init_terms[17];

/* Flag indicating that the adjacency list and initial terms have been
 * built.  All construction is performed lazily on first call. */
static bool ready = false;

/* ------------------------------------------------------------ */
/* Helper to build the adjacency list. */
static void build_adjacency(void)
{
    for (int v = 0; v < ROWS; ++v) {
        uint8_t v_digits[5];
        int v_diff[4];
        for (int j = 0; j < 5; ++j)
            v_digits[j] = (v >> (2 * j)) & 3;
        for (int j = 0; j < 4; ++j)
            v_diff[j] = (int)v_digits[j + 1] - (int)v_digits[j];
        adj_len[v] = 0;

        for (int w = 0; w < ROWS; ++w) {
            uint8_t w_digits[5];
            int w_diff[4];
            for (int j = 0; j < 5; ++j)
                w_digits[j] = (w >> (2 * j)) & 3;
            for (int j = 0; j < 4; ++j)
                w_diff[j] = (int)w_digits[j + 1] - (int)w_digits[j];

            /* Horizontal difference nondecreasing: v_diff <= w_diff */
            bool ok = true;
            for (int j = 0; j < 4 && ok; ++j)
                if (v_diff[j] > w_diff[j]) ok = false;

            /* Vertical absolute difference nondecreasing */
            if (ok) {
                for (int j = 0; j < 4 && ok; ++j) {
                    int av  = abs((int)v_digits[j]   - (int)w_digits[j]);
                    int avp = abs((int)v_digits[j+1] - (int)w_digits[j+1]);
                    if (av > avp) ok = false;
                }
            }

            if (ok) adjacency[v][adj_len[v]++] = (uint16_t)w;
        }
    }
}

/* ------------------------------------------------------------ */
/* Compute the first 17 terms by dynamic programming. */
static void compute_initial_terms(void)
{
    seq_t dp_prev[ROWS];
    seq_t dp_curr[ROWS];

    for (int v = 0; v < ROWS; ++v) dp_prev[v] = (seq_t)1;

    for (int step = 1; step <= 17; ++step) {
        /* Zero current layer */
        for (int v = 0; v < ROWS; ++v) dp_curr[v] = (seq_t)0;

        /* Transition */
        for (int v = 0; v < ROWS; ++v) {
            seq_t val = dp_prev[v];
            if (val == (seq_t)0) continue;
            uint16_t cnt = adj_len[v];
            for (uint16_t k = 0; k < cnt; ++k) {
                int w = adjacency[v][k];
                dp_curr[w] += val;
            }
        }

        /* Sum all end states to obtain a(step) */
        seq_t sum = (seq_t)0;
        for (int v = 0; v < ROWS; ++v) sum += dp_curr[v];
        init_terms[step - 1] = sum;

        /* Prepare for next step */
        for (int v = 0; v < ROWS; ++v) dp_prev[v] = dp_curr[v];
    }
}

/* ------------------------------------------------------------ */
/* The required sequence function.  External linkage, no other
 * side effects, and lazy initialization. */
seq_t a(idx_t n)
{
    if (!ready) {
        build_adjacency();
        compute_initial_terms();
        ready = true;
    }

    if (n <= 17) return init_terms[(int)n - 1];

    /* Sliding window of the last 13 computed terms.  last[0] = a(current) */
    seq_t last[13];
    for (int k = 0; k < 13; ++k)     /* last[0]=a(17), last[12]=a(5) */
        last[k] = init_terms[16 - k];

    for (idx_t cur = 18; cur <= n; ++cur) {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        for (int k = 0; k < 13; ++k) {
            seq_t prod = last[k] * (seq_t)llabs(coeffs[k]);
            if (coeffs[k] > 0) pos += prod;
            else               neg += prod;
        }
        seq_t next = pos - neg;   /* guaranteed non‑negative */

        /* Shift window right and insert new term at front */
        for (int k = 12; k > 0; --k) last[k] = last[k - 1];
        last[0] = next;
    }

    return last[0];
}

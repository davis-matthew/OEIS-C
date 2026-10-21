/*
 * A251814  Number of (n+2)X(3+2) 0..2 arrays with every row, column, diagonal
 *          or antidiagonal in each 3X3 subblock summing to a prime.
 * https://oeis.org/A251814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (see recurrence) with empirical coefficients a(n) =
 *            a(n-1)+a(n-2)+13*a(n-3)+31*a(n-4)+43*a(n-5)+25*a(n-6)
 *            -64*a(n-7)-125*a(n-8)-49*a(n-9)+79*a(n-10)+119*a(n-11)
 *            +20*a(n-12)-53*a(n-13)-42*a(n-14)-12*a(n-15)+6*a(n-16)+4*a(n-17)+4*a(n-18)
 *            For n<=21 the values are computed by a DP over 3×5 arrays
 * Range:     u8 none, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* constants */
#define ROWS 243                 /* 3^5 possible rows */
#define STATE (ROWS*ROWS)        /* (prev_row, curr_row) state count */

/* global data, built once */
static uint8_t row_digits[ROWS][5];
static size_t *edge_start = NULL;     /* prefix array (STATE+1) */
static idx_t *edges = NULL;           /* destination states */
static size_t *out_count = NULL;      /* temp for counting edges */
static bool built = false;

/* primes up to 6 */
static const bool prime[7] = { false, false, true, true, false, true, false };

/* ------------------------------------------------------------------ */
/* Build row digits from base-3 representation */
static void build_row_digits(void)
{
    for (int r = 0; r < ROWS; ++r) {
        int v = r;
        for (int i = 0; i < 5; ++i) {
            row_digits[r][i] = v % 3;
            v /= 3;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Test if a triple of rows satisfies all prime sum constraints
   for each 3×3 subblock (3 columns windows). */
static bool triple_valid(uint8_t r0, uint8_t r1, uint8_t r2)
{
    for (int col = 0; col < 3; ++col) {
        /* row sums */
        int rs0 = row_digits[r0][col] + row_digits[r0][col+1] + row_digits[r0][col+2];
        if (!prime[rs0]) return false;
        int rs1 = row_digits[r1][col] + row_digits[r1][col+1] + row_digits[r1][col+2];
        if (!prime[rs1]) return false;
        int rs2 = row_digits[r2][col] + row_digits[r2][col+1] + row_digits[r2][col+2];
        if (!prime[rs2]) return false;
        /* column sums */
        int cs0 = row_digits[r0][col] + row_digits[r1][col] + row_digits[r2][col];
        if (!prime[cs0]) return false;
        int cs1 = row_digits[r0][col+1] + row_digits[r1][col+1] + row_digits[r2][col+1];
        if (!prime[cs1]) return false;
        int cs2 = row_digits[r0][col+2] + row_digits[r1][col+2] + row_digits[r2][col+2];
        if (!prime[cs2]) return false;
        /* diagonal sums */
        int d1 = row_digits[r0][col] + row_digits[r1][col+1] + row_digits[r2][col+2];
        if (!prime[d1]) return false;
        int d2 = row_digits[r0][col+2] + row_digits[r1][col+1] + row_digits[r2][col];
        if (!prime[d2]) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Build transition graph for DP (adjacency list). */
static void build_adjacency(void)
{
    /* count edges per source */
    out_count = (size_t *)malloc(STATE * sizeof(size_t));
    if (!out_count) return;
    for (size_t s = 0; s < STATE; ++s) out_count[s] = 0;
    for (int prev = 0; prev < ROWS; ++prev) {
        for (int curr = 0; curr < ROWS; ++curr) {
            size_t src = (size_t)prev * ROWS + curr;
            for (int next = 0; next < ROWS; ++next) {
                if (triple_valid((uint8_t)prev, (uint8_t)curr, (uint8_t)next))
                    out_count[src] += 1;
            }
        }
    }
    /* prefix sums for edge starts */
    edge_start = (size_t *)malloc((STATE + 1) * sizeof(size_t));
    if (!edge_start) return;
    edge_start[0] = 0;
    for (size_t s = 0; s < STATE; ++s)
        edge_start[s+1] = edge_start[s] + out_count[s];
    size_t total_edges = edge_start[STATE];

    /* allocate edges array */
    edges = (idx_t *)malloc(total_edges * sizeof(idx_t));
    if (!edges) return;

    /* second pass to populate edges */
    size_t *pos = (size_t *)malloc(STATE * sizeof(size_t));
    if (!pos) return;
    for (size_t s = 0; s < STATE; ++s) pos[s] = edge_start[s];
    for (int prev = 0; prev < ROWS; ++prev) {
        for (int curr = 0; curr < ROWS; ++curr) {
            size_t src = (size_t)prev * ROWS + curr;
            for (int next = 0; next < ROWS; ++next) {
                if (!triple_valid((uint8_t)prev, (uint8_t)curr, (uint8_t)next))
                    continue;
                size_t p = pos[src]++;
                edges[p] = (idx_t)curr * ROWS + next;
            }
        }
    }
    free(pos);
    free(out_count);
    out_count = NULL;
}

/* ------------------------------------------------------------------ */
/* Precompute first 21 terms using DP. */
static seq_t init_terms[21];
static bool init_computed = false;

static void compute_init_terms(void)
{
    if (init_computed) return;
    /* Build everything once */
    build_row_digits();
    build_adjacency();

    /* DP arrays */
    seq_t *dp  = NULL;
    seq_t *dp2 = NULL;
    /* Allocate dp arrays */
#ifdef __cplusplus
    dp = new seq_t[STATE];
    dp2 = new seq_t[STATE];
#else
    dp = (seq_t *)malloc(STATE * sizeof(seq_t));
    dp2 = (seq_t *)malloc(STATE * sizeof(seq_t));
#endif
    if (!dp || !dp2) return;
    for (size_t i = 0; i < STATE; ++i) { dp[i] = 0; dp2[i] = 0; }

    /* init dp with all triples (first 3 rows) */
    for (int r0 = 0; r0 < ROWS; ++r0) {
        for (int r1 = 0; r1 < ROWS; ++r1) {
            for (int r2 = 0; r2 < ROWS; ++r2) {
                if (triple_valid((uint8_t)r0, (uint8_t)r1, (uint8_t)r2)) {
                    size_t src = (size_t)r1 * ROWS + r2; /* state after 3 rows */
                    dp[src] += (seq_t)1;
                }
            }
        }
    }

    /* compute 21 terms (n=1 .. 21) */
    for (size_t step = 0; step < 21; ++step) {
        seq_t total = 0;
        for (size_t s = 0; s < STATE; ++s) total += dp[s];
        init_terms[step] = total;
        if (step == 20) break; /* last term reached */

        /* transition */
        for (size_t s = 0; s < STATE; ++s) {
            if (dp[s] == 0) continue;
            seq_t val = dp[s];
            for (size_t e = edge_start[s]; e < edge_start[s+1]; ++e) {
                dp2[edges[e]] += val;
            }
        }
        /* swap dp and dp2, and zero dp2 */
        seq_t *tmp = dp;
        dp = dp2;
        dp2 = tmp;
        for (size_t i = 0; i < STATE; ++i) dp2[i] = 0;
    }

#ifdef __cplusplus
    delete[] dp;
    delete[] dp2;
#else
    free(dp);
    free(dp2);
#endif
    init_computed = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;     /* undefined for n=0, but guard */

    compute_init_terms();    /* ensures adjacency and init terms ready */

    if ((idx_t)21 >= n) {
        return init_terms[(size_t)(n-1)];
    }

    /* coefficients for recurrence (index 1..18) */
    static const int coeff[19] = {
        0,   /* unused, 0 index */
        1,   /* a(n-1) */
        1,   /* a(n-2) */
        13,  /* a(n-3) */
        31,  /* a(n-4) */
        43,  /* a(n-5) */
        25,  /* a(n-6) */
        -64, /* a(n-7) */
        -125,/* a(n-8) */
        -49, /* a(n-9) */
        79,  /* a(n-10) */
        119, /* a(n-11) */
        20,  /* a(n-12) */
        -53, /* a(n-13) */
        -42, /* a(n-14) */
        -12, /* a(n-15) */
        6,   /* a(n-16) */
        4,   /* a(n-17) */
        4    /* a(n-18) */
    };

    size_t N = (size_t)n;
    seq_t *seq = NULL;
#ifdef __cplusplus
    seq = new seq_t[N];
#else
    seq = (seq_t *)malloc(N * sizeof(seq_t));
    if (!seq) return 0;
#endif
    if (!seq) return 0;

    /* copy initial terms */
    for (size_t i = 0; i < 21; ++i) seq[i] = init_terms[i];

    /* compute further terms */
    for (size_t i = 21; i < N; ++i) {
        seq_t val = 0;
        for (int d = 1; d <= 18; ++d) {
            seq_t term = seq[i-d];
            if (coeff[d] >= 0) {
                val += (seq_t)coeff[d] * term;
            } else {
                val -= (seq_t)(-coeff[d]) * term;
            }
        }
        seq[i] = val;
    }

    seq_t result = seq[N-1];
#ifdef __cplusplus
    delete[] seq;
#else
    free(seq);
#endif
    return result;
}

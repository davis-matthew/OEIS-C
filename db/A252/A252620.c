/*
 * A252620  Number of (5+2)X(n+2) 0..3 arrays with every 3X3 subblock row and
 *          diagonal sum equal to 0 3 5 6 or 8 and every 3X3 column and
 *          antidiagonal sum not equal to 0 3 5 6 or 8.
 * https://oeis.org/A252620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 7×(n+2) arrays over {0,1,2,3} in which every *
 *            3×3 subblock has each row sum and main diagonal sum belonging to
 *            * {0,3,5,6,8} and each column sum and anti‑diagonal sum not
 *            belonging to * that set. The sequence is computed by exhaustive
 *            enumeration of all * admissible 7×3 subblock...
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 26, u128 n <= 58,
 *            big any n
 * Helpers:   encode_col, inS
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/encode_col.h"
#include "../utils/inS.h"

static const int S[5] = {0,3,5,6,8};

/* ----------  Parameters  ------------------------------------------- */
#define MAX_TRIPLES 5000
#define MAX_PAIRS   5000

/* ----------  Global tables (filled on first call) ------------------ */
static uint16_t triple_c1[MAX_TRIPLES], triple_c2[MAX_TRIPLES], triple_c3[MAX_TRIPLES];
static int triple_count = 0;

static int p1_of_triple[MAX_TRIPLES], p2_of_triple[MAX_TRIPLES];

static uint16_t pair_c1[MAX_PAIRS], pair_c2[MAX_PAIRS];
static int pair_count = 0;
static int dest_cnt[MAX_PAIRS];
static int offset_start[MAX_PAIRS+1];
static uint16_t adj_dest[MAX_TRIPLES];   /* destination pair index for each edge   */
static int temp_pos[MAX_PAIRS];

static uint8_t allowed_tuples[36][3];
static int allowed_cnt = 0;

static uint8_t col[3][7];          /* current column values while building triples */

static seq_t cur_counts[MAX_PAIRS];
static seq_t next_counts[MAX_PAIRS];

static bool initialized = false;

static int get_pair_index(uint16_t a, uint16_t b) {
    for(int i = 0; i < pair_count; ++i)
        if(pair_c1[i]==a && pair_c2[i]==b) return i;
    pair_c1[pair_count] = a;
    pair_c2[pair_count] = b;
    return pair_count++;
}

/* ----------  DFS to generate 7×3 admissible subblocks  ------------ */
static void dfs_rows(int r) {
    if(r == 7) {
        triple_c1[triple_count] = encode_col(col[0]);
        triple_c2[triple_count] = encode_col(col[1]);
        triple_c3[triple_count] = encode_col(col[2]);
        ++triple_count;
        return;
    }
    for(int t = 0; t < allowed_cnt; ++t) {
        col[0][r] = allowed_tuples[t][0];
        col[1][r] = allowed_tuples[t][1];
        col[2][r] = allowed_tuples[t][2];

        if(r >= 2) {
            int rr = r - 2;
            int diag = col[0][rr] + col[1][rr+1] + col[2][rr+2];
            if(!inS(diag)) continue;
            bool bad = false;
            for(int k = 0; k < 3; ++k) {
                int sumcol = col[k][rr] + col[k][rr+1] + col[k][rr+2];
                if(inS(sumcol)) { bad = true; break; }
            }
            if(bad) continue;
            int anti = col[0][rr+2] + col[1][rr+1] + col[2][rr];
            if(inS(anti)) continue;
        }
        dfs_rows(r+1);
    }
}

/* ----------  Initialization --------------------------------------- */
static void init(void) {
    if(initialized) return;
    /* 1. allowed tuples for a single row (row sum in S) */
    for(int a = 0; a < 4; ++a)
        for(int b = 0; b < 4; ++b)
            for(int c = 0; c < 4; ++c) {
                int s = a+b+c;
                if(inS(s)) {
                    allowed_tuples[allowed_cnt][0] = a;
                    allowed_tuples[allowed_cnt][1] = b;
                    allowed_tuples[allowed_cnt][2] = c;
                    ++allowed_cnt;
                }
            }

    /* 2. enumerate all admissible 7×3 subblocks */
    dfs_rows(0);

    /* 3. build pair indices and adjacency lists */
    int i;
    for(i = 0; i < triple_count; ++i) {
        int p1 = get_pair_index(triple_c1[i], triple_c2[i]);
        int p2 = get_pair_index(triple_c2[i], triple_c3[i]);
        p1_of_triple[i] = p1;
        p2_of_triple[i] = p2;
        ++dest_cnt[p1];
    }

    /* prefix sums for adjacency offsets */
    offset_start[0] = 0;
    for(i = 0; i < pair_count; ++i)
        offset_start[i+1] = offset_start[i] + dest_cnt[i];

    /* place edges */
    for(i = 0; i < pair_count; ++i) temp_pos[i] = 0;
    for(i = 0; i < triple_count; ++i) {
        int p1 = p1_of_triple[i];
        int p2 = p2_of_triple[i];
        int idx = offset_start[p1] + temp_pos[p1]++;
        adj_dest[idx] = (uint16_t)p2;
    }

    initialized = true;
}

/* ----------  Main sequence function ------------------------------- */
seq_t a(idx_t n) {
    init();
    if(n < 1) return 0;            /* below offset – behaviour undefined */
    if(n == 1) return (seq_t)triple_count;   /* a(1) = number of admissible 7×3 blocks */

    size_t steps = (size_t)(n-1);   /* number of additional columns beyond the first 3 */

    /* init DP array with initial pair counts */
    for(int i = 0; i < pair_count; ++i) cur_counts[i] = 0;
    for(int i = 0; i < triple_count; ++i) {
        int pStart = p2_of_triple[i];   /* pair (c2,c3) */
        cur_counts[pStart] += 1;
    }

    /* run transitions */
    for(size_t step = 0; step < steps; ++step) {
        for(int i = 0; i < pair_count; ++i) next_counts[i] = 0;
        for(int i = 0; i < pair_count; ++i) {
            seq_t cnt = cur_counts[i];
            if(cnt == 0) continue;
            int start = offset_start[i];
            int end   = offset_start[i+1];
            for(int e = start; e < end; ++e) {
                int dest = adj_dest[e];
                next_counts[dest] += cnt;
            }
        }
        for(int i = 0; i < pair_count; ++i) cur_counts[i] = next_counts[i];
    }

    /* sum all final counts */
    seq_t result = 0;
    for(int i = 0; i < pair_count; ++i) result += cur_counts[i];
    return result;
}

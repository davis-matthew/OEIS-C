/*
 * A290778  Number of connected undirected unlabeled loopless multigraphs with
 *          4 vertices and n edges.
 * https://oeis.org/A290778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the number of connected undirected unlabeled loopless
 *            multigraphs on 4 vertices with n edges. The value is obtained by
 *            summing, over the six underlying simple graphs, the average
 *            number of edge-multiplicity assignments fixed by the
 *            automorphism group of each underlying graph. This is a di...
 * Range:     u8 n <= 9, u16 n <= 26, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

#define MAX_K 20000     /* sufficient for n up to 10000 */

/* ======================================================================
   Cycle index data for each underlying simple graph:
   ====================================================================== */

/* 1. Path P4 (3 edges) */
static const int L1_P4[3] = {1,1,1};      /* identity */
static const int L2_P4[2] = {2,1};        /* reflection */

/* 2. Star S3 (3 edges) */
static const int L1_S3[3] = {1,1,1};
static const int L2_S3[2] = {2,1};
static const int L3_S3[1] = {3};

/* 3. Square C4 (4 edges) */
static const int L1_C4[4] = {1,1,1,1};
static const int L2_C4[1] = {4};
static const int L3_C4[2] = {2,2};
static const int L4_C4[3] = {2,1,1};

/* 4. Triangle with pendant leaf (4 edges) */
static const int L1_T[4] = {1,1,1,1};
static const int L2_T[3] = {2,1,1};

/* 5. Diamond (K4 minus one edge) (5 edges) */
static const int L1_D[5] = {1,1,1,1,1};
static const int L2_D[3] = {2,2,1};

/* 6. Complete graph K4 (6 edges) */
static const int L1_K4[6] = {1,1,1,1,1,1};
static const int L2_K4[4] = {2,2,1,1};
static const int L3_K4[2] = {3,3};
static const int L4_K4[2] = {4,2};

/* ======================================================================
   Helper: coefficient of x^k in product (1 - x^L_i)^-1 for a given set
   of cycle lengths L_i.
   ====================================================================== */
static seq_t coeff(const int *lens, int count, int k)
{
    if (k < 0) return 0;
    static seq_t dp[MAX_K + 1];
    for (int i = 0; i <= k; ++i) dp[i] = 0;
    dp[0] = 1;
    for (int idx = 0; idx < count; ++idx) {
        int L = lens[idx];
        for (int s = L; s <= k; ++s) {
            dp[s] += dp[s - L];
        }
    }
    return dp[k];
}

/* ======================================================================
   Main sequence function.
   ====================================================================== */
seq_t a(idx_t n)
{
    seq_t total = 0;

    /* 1. Path P4 (base 3 edges) */
    if (n >= 3) {
        int k = (int)(n - 3);
        seq_t val = coeff(L1_P4, 3, k) + coeff(L2_P4, 2, k);
        total += val / 2;
    }

    /* 2. Star S3 (base 3 edges) */
    if (n >= 3) {
        int k = (int)(n - 3);
        seq_t val = coeff(L1_S3, 3, k)
                  + 3 * coeff(L2_S3, 2, k)
                  + 2 * coeff(L3_S3, 1, k);
        total += val / 6;
    }

    /* 3. Square C4 (base 4 edges) */
    if (n >= 4) {
        int k = (int)(n - 4);
        seq_t val = coeff(L1_C4, 4, k)
                  + 2 * coeff(L2_C4, 1, k)
                  + 3 * coeff(L3_C4, 2, k)
                  + 2 * coeff(L4_C4, 3, k);
        total += val / 8;
    }

    /* 4. Triangle + pendant leaf (base 4 edges) */
    if (n >= 4) {
        int k = (int)(n - 4);
        seq_t val = coeff(L1_T, 4, k)
                  + coeff(L2_T, 3, k);
        total += val / 2;
    }

    /* 5. Diamond (K4 minus one edge) (base 5 edges) */
    if (n >= 5) {
        int k = (int)(n - 5);
        seq_t val = coeff(L1_D, 5, k)
                  + 3 * coeff(L2_D, 3, k);
        total += val / 4;
    }

    /* 6. Complete graph K4 (base 6 edges) */
    if (n >= 6) {
        int k = (int)(n - 6);
        seq_t val = coeff(L1_K4, 6, k)
                  + 9 * coeff(L2_K4, 4, k)
                  + 8 * coeff(L3_K4, 2, k)
                  + 6 * coeff(L4_K4, 2, k);
        total += val / 24;
    }

    return total;
}

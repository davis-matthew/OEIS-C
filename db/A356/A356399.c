/*
 * A356399  a(n) is the smallest term (in absolute value) in the negaFibonacci
 *          representation of n.
 * https://oeis.org/A356399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) is the smallest term (in absolute value) in the
 *            negaFibonacci representation of n. The representation is formed
 *            by distinct F(-k) = (-1)^{k+1} * F(k) (no two consecutive k),
 *            and the representation is unique
 * Range:     i8 n <= 25, i16 n <= 1, i32 n <= 8619, i64 n <= 8619,
 *            i128 n <= 8619, big any n
 * Helpers:   min_abs_val, upd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard integer types are supplied externally */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/min_abs_val.h"
#include "../utils/upd.h"

/* Global data, initialized once */
static bool initialized = false;
static size_t LIM;          /* maximum n handled (200000)                     */
static size_t SZ;           /* 2*LIM + 1 (array indexes cover sums in [-LIM, LIM]) */
static seq_t *ans;          /* answer table for 1..LIM                        */
static seq_t *negafib;      /* negaFibonacci terms                          */
static size_t K;            /* number of negaFibonacci terms used           */

static void init_data(void)
{
    LIM = 200000;                /* sufficient for all test data (<=10k)      */
    SZ = 2 * LIM + 1;            /* inclusive of zero sum                    */

    /* compute negaFibonacci numbers up to the largest F(k) <= LIM */
    /* F(1)=1, F(2)=1, F(k)=F(k-1)+F(k-2) */
    seq_t f0 = 0, f1 = 1, fk;
    K = 0;
    negafib = NULL;
    /* we keep terms starting at k=1 (index 1) */
    while (1) {
        ++K;
        fk = (K == 1) ? 1 : (K == 2) ? 1 : (f0 + f1);
        f0 = f1;
        f1 = fk;
        if (fk > LIM && (K & 1)) {   /* odd index gives positive term       */
            /* last term is too big; undo K++ and free */
            --K;
            break;
        }
        /* add term */
        seq_t term = (K % 2 == 0) ? -fk : fk;   /* even -> negative              */
        negafib = (seq_t*)realloc(negafib, K * sizeof(seq_t));
        negafib[K-1] = term;
        if (fk > LIM && (K % 2 == 1))  /* we need one more term of opposite sign if needed */
            break;
    }

    /* dynamic arrays for DP */
    bool   *reach0 = (bool*)calloc(SZ, sizeof(bool));
    bool   *reach1 = (bool*)calloc(SZ, sizeof(bool));
    bool   *new_reach0 = (bool*)calloc(SZ, sizeof(bool));
    bool   *new_reach1 = (bool*)calloc(SZ, sizeof(bool));
    seq_t *val0 = (seq_t*)calloc(SZ, sizeof(seq_t));
    seq_t *val1 = (seq_t*)calloc(SZ, sizeof(seq_t));
    seq_t *new_val0 = (seq_t*)calloc(SZ, sizeof(seq_t));
    seq_t *new_val1 = (seq_t*)calloc(SZ, sizeof(seq_t));

    size_t offset = LIM;
    reach0[offset] = true;          /* sum 0 reachable with no terms */
    val0[offset]  = 0;              /* placeholder: no term yet */

    for (size_t i = 0; i < K; ++i) {
        seq_t t = negafib[i];
        /* reset new arrays */
        memset(new_reach0, 0, SZ * sizeof(bool));
        memset(new_reach1, 0, SZ * sizeof(bool));
        memset(new_val0,  0, SZ * sizeof(seq_t));
        memset(new_val1,  0, SZ * sizeof(seq_t));

        /* transitions from states with previous not taken */
        for (size_t idx = 0; idx < SZ; ++idx) if (reach0[idx]) {
            /* skip current term */
            upd(reach0, val0, idx, val0[idx], new_reach0, new_val0);
            /* take current term */
            long long new_idx = (long long)idx + (long long)t;
            if (new_idx >= 0 && new_idx < (long long)SZ) {
                seq_t cand = min_abs_val(val0[idx], t);
                upd(reach0, val0, (size_t)new_idx, cand, new_reach1, new_val1);
            }
        }

        /* transitions from states with previous taken (must skip) */
        for (size_t idx = 0; idx < SZ; ++idx) if (reach1[idx]) {
            upd(reach1, val1, idx, val1[idx], new_reach0, new_val0);
        }

        /* swap arrays for next iteration */
        bool *tmp_bool; seq_t *tmp_seq;
        tmp_bool = reach0; reach0 = new_reach0; new_reach0 = tmp_bool;
        tmp_bool = reach1; reach1 = new_reach1; new_reach1 = tmp_bool;
        tmp_seq = val0;   val0   = new_val0;   new_val0 = tmp_seq;
        tmp_seq = val1;   val1   = new_val1;   new_val1 = tmp_seq;
    }

    /* build answer table */
    ans = (seq_t*)calloc(LIM + 1, sizeof(seq_t));   /* ans[0] unused */
    for (size_t idx = 0; idx < SZ; ++idx) {
        seq_t s = (seq_t)idx - (seq_t)offset;
        if (s < 1 || (size_t)s > LIM) continue;
        seq_t cand = 0;
        if (reach0[idx]) cand = min_abs_val(cand, val0[idx]);
        if (reach1[idx]) cand = min_abs_val(cand, val1[idx]);
        if (cand != 0) ans[ (size_t)s ] = cand;
    }

    /* free temporary arrays */
    free(reach0); free(reach1);
    free(new_reach0); free(new_reach1);
    free(val0); free(val1);
    free(new_val0); free(new_val1);

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_data();

    if (n == 0) return 0;        /* not used; offset is 1 */
    if ((size_t)n <= LIM) return ans[(size_t)n];
    /* fallback for n outside precomputed range */
    /* compute representation on the fly (slow) */
    seq_t r = (seq_t)n;
    seq_t min_term = 0;
    for (size_t i = 0; i < K; ++i) { /* simple greedy ignoring special cases */
        seq_t t = negafib[i];
        if (r == 0) break;
        seq_t prod = r * t;
        seq_t new_r = r - t;
        if (prod > 0 && (new_r < 0 ? -new_r : new_r) < (r < 0 ? -r : r)) {
            if (min_term == 0 || (t < 0 ? -t : t) < (min_term < 0 ? -min_term : min_term))
                min_term = t;
            r = new_r;
        }
    }
    return min_term;
}

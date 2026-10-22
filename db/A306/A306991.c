/*
 * A306991  Number of primitive Heronian isosceles triangles with perimeter
 *          A308222(n).
 * https://oeis.org/A306991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A306991
   Number of primitive Heronian isosceles triangles with perimeter A308222(n).
   We generate all primitive parameters (m,n) with m>n>0, gcd(m,n)=1,
   m and n of opposite parity.  For each pair we produce two
   primitive triangles (using the two possible bases) and
   collect their perimeters.  The count of triangles sharing the same
   perimeter gives the value of the sequence.  The list of perimeters
   is sorted ascending, and a(n) is the multiplicity of the nth unique
   perimeter.  The algorithm is memoised, so calls to a() for any
   index are fast.  All arithmetic uses 'seq_t' to preserve exactness. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* comparison function for qsort on seq_t values */
static int seq_cmp(const void *p, const void *q)
{
    seq_t av = *(const seq_t *)p;
    seq_t bv = *(const seq_t *)q;
    if (av < bv) return -1;
    if (av > bv) return 1;
    return 0;
}

/* ------------------------------------------------------------------ */
/*  Memoised data structures                                          */
/* ------------------------------------------------------------------ */
static seq_t *raw   = NULL;      /* raw perimeters (with duplicates) */
static size_t raw_sz = 0;
static size_t raw_cap = 0;

static seq_t *perc  = NULL;      /* unique perimeters */
static size_t *cnt   = NULL;     /* multiplicities */
static size_t perc_sz = 0;
static size_t perc_cap = 0;

static idx_t max_proc_m = 0;     /* largest m processed */

/* ------------------------------------------------------------------ */
/*  Generate primitive triangles up to a required number of unique
      perimeters.  The routine expands the raw list and rebuilds
      the compressed arrays when needed.                                     */
/* ------------------------------------------------------------------ */
static void ensure_computed(size_t need)
{
    if (perc_sz >= need) return;

    /* grow raw array to accommodate new perimeters */
    const size_t STEP = 200;          /* m step per iteration */
    while (perc_sz < need) {
        idx_t new_m = max_proc_m + STEP;

        /* compute number of new pairs: approximate */
        size_t add_cnt = 0;
        for (idx_t m = max_proc_m + 1; m <= new_m; ++m) {
            for (idx_t n = 1; n < m; ++n) {
                if (!((m ^ n) & 1)) continue;           /* need opposite parity */
                if (gcd((seq_t)m, (seq_t)n) != 1) continue;
                add_cnt += 2;  /* two base options */
            }
        }

        /* reallocate raw buffer if needed */
        if (raw_sz + add_cnt > raw_cap) {
            size_t new_cap = raw_cap ? raw_cap * 2 : 16;
            while (new_cap < raw_sz + add_cnt) new_cap *= 2;
            raw = (seq_t *)realloc(raw, new_cap * sizeof(seq_t));
            raw_cap = new_cap;
        }

        /* generate perimeters for new m-range */
        for (idx_t m = max_proc_m + 1; m <= new_m; ++m) {
            seq_t mm2 = (seq_t)m * (seq_t)m;
            for (idx_t n = 1; n < m; ++n) {
                if (!((m ^ n) & 1)) continue;
                if (gcd((seq_t)m, (seq_t)n) != 1) continue;

                seq_t nn2 = (seq_t)n * (seq_t)n;
                seq_t a   = mm2 + nn2;                     /* equal side a */
                /* two possible bases */
                seq_t b1  = 2 * (mm2 - nn2);               /* 2*(m^2-n^2)  */
                seq_t b2  = 4 * (seq_t)m * (seq_t)n;       /* 4*m*n        */

                /* store perimeters (both primitive) */
                raw[raw_sz++] = 2 * a + b1;
                raw[raw_sz++] = 2 * a + b2;
            }
        }
        max_proc_m = new_m;

        /* sort raw perimeters so we can compress duplicates */
        qsort(raw, raw_sz, sizeof(seq_t), seq_cmp);

        /* compress into unique list */
        size_t i = 0, k = 0;
        while (i < raw_sz) {
            size_t j = i + 1;
            while (j < raw_sz && raw[j] == raw[i]) ++j;

            if (k >= perc_cap) {
                size_t new_cap = perc_cap ? perc_cap * 2 : 16;
                perc  = (seq_t  *)realloc(perc,  new_cap * sizeof(seq_t));
                cnt   = (size_t *)realloc(cnt,   new_cap * sizeof(size_t));
                perc_cap = new_cap;
            }
            perc[k] = raw[i];
            cnt[k]  = j - i;
            ++k; i = j;
        }
        perc_sz = k;
    }
}

/* ------------------------------------------------------------------ */
/*  Sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* sequence starts at index 1 */
    ensure_computed((size_t)n);
    return (seq_t)cnt[(size_t)n - 1];
}

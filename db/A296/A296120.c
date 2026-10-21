/*
 * A296120  Number of ways to choose a strict factorization of each factor in
 *          a strict factorization of n.
 * https://oeis.org/A296120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   Gcalc_large, Scalc_large
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A296120
 *  Formula:  a(n) = sum_{strict factorizations of n}  Π_{f in factorization} g(f)
 *  where g(f) is the number of strict factorizations of f.
 *
 *  The implementation uses memoised recursion for g(f) (stored in Gmem)
 *  and for the summation function (stored in Smem).  A cache for a(n)
 *  itself is also maintained.  All arithmetic is performed on type seq_t,
 *  so the code works for 8‑, 16‑, 32‑, 64‑bit unsigned integers and for
 *  arbitrary‑precision integers in C++ builds.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/Gcalc_large.h"
#include "../utils/Scalc_large.h"

/* ------------------------------------------------------------------ */
/*  Configuration:  maximum n supported with memoised tables.  The
 *  implementation will handle any larger n with a non‑cached
 *  recursion, which is slower but sufficient for the test range
 *  (≤ 10000).  Increase MAXN only if you need to support larger n,
 *  keeping in mind the memory consumption grows roughly linearly with
 *  MAXN * sqrt(MAXN).
 */
#define MAXN 20000
static const size_t WIDTH = (size_t)floor(sqrt((double)MAXN)) + 3;

/* ------------------------------------------------------------------ */
/*  Static arrays for memoisation.  They are allocated on the first
 *  call to a().
 */
static seq_t *Gmem = NULL;   /* result of Gcalc */
static bool  *Gvis = NULL;   /* visited flag for Gmem */
static seq_t *Smem = NULL;   /* result of Scalc */
static bool  *Svis = NULL;   /* visited flag for Smem */
static seq_t *Anum = NULL;   /* cached a(n) values */
static bool  *Avis = NULL;   /* flag if a(n) cached */

/* ------------------------------------------------------------------ */
static void init(void)
{
    /* allocate memoisation tables */
    size_t gsize = (MAXN + 1) * WIDTH;
    Gmem = (seq_t*)malloc(gsize * sizeof(seq_t));
    Gvis = (bool*)calloc(gsize, sizeof(bool));

    Smem = (seq_t*)malloc(gsize * sizeof(seq_t));
    Svis = (bool*)calloc(gsize, sizeof(bool));

    Anum = (seq_t*)calloc(MAXN + 1, sizeof(seq_t));
    Avis = (bool*)calloc(MAXN + 1, sizeof(bool));
}

/* ------------------------------------------------------------------ */
/*  Recursive computation of g(n)=number of strict factorizations of n,
 *  with all factors ≥minf.  Minf=2 gives the full count g(n).
 */
static seq_t Gcalc(idx_t n, idx_t minf)
{
    if (n < minf || n == 1) return 0;

    size_t idx = (size_t)n * WIDTH + minf;
    if (Gvis[idx]) return Gmem[idx];

    seq_t sum = 1; /* the trivial factorization (n) itself */

    idx_t lim = (idx_t)floor(sqrt((double)n));
    for (idx_t f = minf; f <= lim; ++f) {
        if (n % f == 0) {
            idx_t g = n / f;
            if (f <= g) /* strictly increasing requires f ≤ g */
                sum += Gcalc(g, f + 1);
        }
    }

    Gvis[idx] = true;
    Gmem[idx] = sum;
    return sum;
}

/* ------------------------------------------------------------------ */
/*  Recursive computation of the a(n) summation for a given minimum
 *  factor minf.  The top level call is Scalc(n, 2).
 */
static seq_t Scalc(idx_t n, idx_t minf)
{
    if (n == 1) return 1;      /* empty factorization: product=1 */
    if (n < minf) return 0;

    size_t idx = (size_t)n * WIDTH + minf;
    if (Svis[idx]) return Smem[idx];

    seq_t sum = Gcalc(n, 2);   /* trivial factorization (n) */

    idx_t lim = (idx_t)floor(sqrt((double)n));
    for (idx_t f = minf; f <= lim; ++f) {
        if (n % f == 0) {
            idx_t g = n / f;
            if (f <= g) {
                seq_t sub = Scalc(g, f + 1);
                sum += Gcalc(f, 2) * sub;
            }
        }
    }

    Svis[idx] = true;
    Smem[idx] = sum;
    return sum;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  The required public function.  External linkage, no static keyword. */
seq_t a(idx_t n)
{
    static bool ready = false;
    if (!ready) { init(); ready = true; }

    if (n <= (idx_t)MAXN) {
        if (!Avis[n]) {
            Anum[n] = Scalc(n, 2);
            Avis[n] = true;
        }
        return Anum[n];
    }
    /* fallback for very large n */
    return Scalc_large(n, 2);
}

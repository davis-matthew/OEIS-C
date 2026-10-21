/*
 * A265199  Triangle read by rows: T(p,q) is the number of B-diagrams G such
 *          that omega(G)=p half edges and h_f(G)=q non-used outer non-cut
 *          half edges.
 * https://oeis.org/A265199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A265199 – Triangle read by rows:
   T(p,q) is the number of B‑diagrams G such that
   omega(G) = p half edges and h_f(G) = q non‑used outer non‑cut half edges.
   T(0,0)=1, and for p,q>0
   T(p,q)=Σ_{i=1..p} Σ_{j=0..i} Σ_{k=0..i} Σ_{l=0..j}
            l! * C(j,l) * C(q-k+l,l) * C(i,j) * C(i,k) * T(p-i, q-k+l).   */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) returns seq_t */

/* ----- factorial cache ----- */
static seq_t *fact = NULL;          /* fact[n] = n!  (seq_t) */
static idx_t factN = 0;             /* highest factorial computed */

/* ensure factorials up to 'up_to' are ready */
static void ensure_factorial(idx_t up_to)
{
    if (up_to <= factN) return;

    idx_t new_size = up_to + 1;
    seq_t *new_fact;

    if (!fact) {
        new_fact = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (!new_fact) exit(1);
        /* base case 0! = 1 */
        new_fact[0] = (seq_t)1;
        for (idx_t i = 1; i <= up_to; ++i)
            new_fact[i] = new_fact[i-1] * (idx_t)i;
        fact = new_fact;
        factN = up_to;
    } else {
        new_fact = (seq_t*)realloc(fact, new_size * sizeof(seq_t));
        if (!new_fact) exit(1);
        for (idx_t i = factN + 1; i <= up_to; ++i)
            new_fact[i] = new_fact[i-1] * (idx_t)i;
        fact = new_fact;
        factN = up_to;
    }
}

/* ----- triangle cache ----- */
static seq_t **T = NULL;            /* T[p][q] values */
static bool **Tcomp = NULL;         /* Tcomp[p][q] == true iff T[p][q] computed */
static idx_t Tsize = 0;             /* number of rows already allocated */

/* ensure arrays can hold rows up to 'max_p' */
static void ensure_T_size(idx_t max_p)
{
    if (Tsize > max_p) return;

    idx_t new_size = max_p + 1;
    seq_t **new_T = (seq_t**)realloc(T, new_size * sizeof(seq_t*));
    bool **new_Tc = (bool**)realloc(Tcomp, new_size * sizeof(bool*));
    if (!new_T || !new_Tc) exit(1);

    for (idx_t p = Tsize; p <= max_p; ++p) {
        new_T[p]    = (seq_t*)calloc(p + 1, sizeof(seq_t));
        new_Tc[p]   = (bool*)calloc(p + 1, sizeof(bool));
        if (!new_T[p] || !new_Tc[p]) exit(1);
    }
    T = new_T;
    Tcomp = new_Tc;
    Tsize = new_size;
}

/* recursive computation of T(p,q) with memoisation */
static seq_t compute_T(idx_t p, idx_t q)
{
    ensure_T_size(p);
    if (Tcomp[p][q]) return T[p][q];

    if (p == 0 && q == 0) {
        T[p][q] = (seq_t)1;
        Tcomp[p][q] = true;
        return T[p][q];
    }

    /* the largest factorial we will need is p! */
    ensure_factorial(p);

    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i <= p; ++i) {
        /* pre‑compute C(i,j) and C(i,k) as they are reused */
        for (idx_t j = 0; j <= i; ++j) {
            seq_t Cij = binomial(i, j);            /* C(i,j) */
            for (idx_t k = 0; k <= i; ++k) {
                seq_t Cik = binomial(i, k);        /* C(i,k) */
                for (idx_t l = 0; l <= j; ++l) {
                    int diff = (int)q - (int)k + (int)l;
                    if (diff < 0) continue;
                    idx_t diff_idx = (idx_t)diff;
                    if (diff_idx > p - i) continue; /* T out of range */

                    seq_t term = fact[l];           /* l! */
                    term *= binomial(j, l);          /* *C(j,l) */
                    term *= binomial(diff_idx, l);   /* *C(q-k+l,l) */
                    term *= Cij;                     /* *C(i,j) */
                    term *= Cik;                     /* *C(i,k) */
                    term *= compute_T(p - i, diff_idx);/* *T(...) */
                    sum += term;
                }
            }
        }
    }

    T[p][q] = sum;
    Tcomp[p][q] = true;
    return sum;
}

seq_t a(idx_t n)
{
    /* find row p such that p*(p+1)/2 <= n < (p+1)*(p+2)/2 */
    idx_t p = 0;
    while ((p + 1) * (p + 2) / 2 <= n) ++p;

    idx_t before = p * (p + 1) / 2;
    idx_t q = n - before;

    return compute_T(p, q);
}

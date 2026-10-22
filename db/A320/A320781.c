/*
 * A320781  Inverse Euler transform of the Moebius function A008683.
 * https://oeis.org/A320781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = the inverse Euler transform of the Möbius function (OEIS
 *            A320781). The sequence starts with a(1)=1, a(2)=-2, a(3)=0, …
 * Range:     i8 n <= 14, i16 n <= 25, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*==========================================================================*/
/*  Standard headers                                                    */
/*==========================================================================*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* The build supplies the types seq_t (signed integer, may be big) and
   idx_t (unsigned integer, can be uint64_t).  They are declared externally
   by the build system, so we do not define them here. */
extern seq_t a(idx_t n);

/*==========================================================================*/
/*  Local data structures                                               */
/*==========================================================================*/
static seq_t *A = NULL;          /* a[n]  */
static seq_t *Tarr = NULL;       /* T[n] = n*g[n]   (g is coefficient of log) */
static seq_t *muArr = NULL;      /* Möbius function values */
static size_t cap = 0;           /* allocated array capacity (>= max index+1) */
static idx_t current_max = 0;    /* maximum index for which data is ready */

/*==========================================================================*/
/*  Utility: integer square‑root                                         */
/*==========================================================================*/
static inline idx_t isqrt_idx(idx_t n) {
    if (n <= 1) return n;
    idx_t r = (idx_t) sqrt((double)n);
    while ((r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    return r;
}

/*==========================================================================*/
/*  Extend tables up to index n (inclusive)                               */
/*==========================================================================*/
static void extend(idx_t n) {
    if (n <= current_max) return;

    /* Expand storage if necessary */
    size_t needed = (size_t)n + 1;
    if (cap < needed) {
        A      = (seq_t*) realloc(A,        sizeof(seq_t) * needed);
        Tarr   = (seq_t*) realloc(Tarr,     sizeof(seq_t) * needed);
        muArr  = (seq_t*) realloc(muArr,    sizeof(seq_t) * needed);
        cap = needed;
        for (size_t i = current_max + 1; i < needed; ++i) {
            A[i]     = 0;
            Tarr[i]  = 0;
            muArr[i] = 0;
        }
    }

    /* Möbius function up to n via linear sieve */
    static bool *isComp = NULL;
    static size_t compCap = 0;
    size_t newCap = needed;
    if (compCap < newCap) {
        isComp = (bool*) realloc(isComp, sizeof(bool) * newCap);
        compCap = newCap;
    }
    for (size_t i = 0; i < newCap; ++i) isComp[i] = false;

    muArr[1] = 1;
    int *primes = (int*) malloc(sizeof(int) * (n + 1));
    int primesCnt = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (!isComp[i]) {
            primes[primesCnt++] = (int)i;
            muArr[i] = -1;
        }
        for (int pi = 0; pi < primesCnt; ++pi) {
            idx_t p = (idx_t) primes[pi];
            idx_t mult = i * p;
            if (mult > n) break;
            isComp[mult] = true;
            if (i % p == 0) {
                muArr[mult] = 0;
                break;
            } else {
                muArr[mult] = -muArr[i];
            }
        }
    }
    free(primes);

    /* Compute Tarr[i] for new indices */
    for (idx_t i = current_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k < i; ++k) {
            sum += Tarr[k] * muArr[i - k];
        }
        Tarr[i] = (seq_t)i * muArr[i] - sum;
    }

    /* Compute a[i] for new indices */
    for (idx_t i = current_max + 1; i <= n; ++i) {
        seq_t divSum = 0;
        idx_t r = isqrt_idx(i);
        for (idx_t d = 1; d <= r; ++d) {
            if (i % d == 0) {
                idx_t e = i / d;
                if (d < i) divSum += A[d] * (seq_t)d;
                if (e != d && e < i) divSum += A[e] * (seq_t)e;
            }
        }
        A[i] = (Tarr[i] - divSum) / (seq_t)i;
    }

    current_max = n;
}

/*==========================================================================*/
/*  Public API: return a(n)                                               */
/*==========================================================================*/
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* safety: offset is 1 */
    extend(n);
    return A[n];
}

/*
 * A294393  a(n) is the smallest number whose deficiency or abundance is equal
 *          to 2*n (or 0 if such a number does not exist), minus the n-th odd
 *          number.
 * https://oeis.org/A294393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A294393
    a(n) = A294386(n) - (2*n + 1)
    A294386(n) = smallest m with |sigma(m) – 2*m| = 2*n, or 0 if none.
    We pre‑compute sigma up to LIMIT_L and record the minimal m for all
    n up to LIMIT_N.  The computation is performed once on the first
    call, then results are returned directly.  The program is fully
    compliant with the requirements for both C11 and C++17.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Constants                                                         */
/* --------------------------------------------------------------------- */
#define LIMIT_L 5000000ULL     /* maximum m to sieve (1e6 gives very small  */
/* to be safe for n up to 10 000 this value is sufficient)           */
#define LIMIT_N 2000000ULL     /* maximum n value that is recorded          */

/* --------------------------------------------------------------------- */
/*  Global data                                                      */
/* --------------------------------------------------------------------- */
static unsigned long long *sigma = NULL;      /* sum of divisors                */
static long long *best    = NULL;              /* minimal m for each n           */
static bool initialized  = false;              /* first call flag                */

/* --------------------------------------------------------------------- */
/*  Helper: initialise data structures and perform the sieve         */
/* --------------------------------------------------------------------- */
static void initialise(void)
{
    if (initialized) return;
    initialized = true;

    /* --- Allocate arrays ------------------------------------------------*/
    sigma = (unsigned long long *)malloc((LIMIT_L + 1) * sizeof(unsigned long long));
    if (!sigma) exit(EXIT_FAILURE);
    for (unsigned long long i = 0; i <= LIMIT_L; ++i) sigma[i] = 0;

    best  = (long long *)malloc((LIMIT_N + 1) * sizeof(long long));
    if (!best) exit(EXIT_FAILURE);
    for (unsigned long long i = 0; i <= LIMIT_N; ++i) best[i] = 0;

    /* --- Compute sigma --------------------------------------------------*/
    for (unsigned long long i = 1; i <= LIMIT_L; ++i) {
        for (unsigned long long j = i; j <= LIMIT_L; j += i) {
            sigma[j] += i;
        }
    }

    /* --- Record minimal m for each n -----------------------------------*/
    for (unsigned long long m = 1; m <= LIMIT_L; ++m) {
        int64_t diff = (int64_t)sigma[m] - (int64_t)2 * (int64_t)m;   /* signed */
        int64_t absdiff = diff >= 0 ? diff : -diff;
        if (absdiff & 1LL) continue;   /* odd differences cannot be 2*n */
        unsigned long long n = (unsigned long long)(absdiff / 2);
        if (n <= LIMIT_N && best[n] == 0) {
            best[n] = (long long)m;
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Public function exported by the build                             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    initialise();

    if (n > LIMIT_N) /* out of computed range */
        return 0;

    long long m = best[n];
    if (m == 0)                 /* no such number found */
        return 0;

    /* odd = 2*n + 1, cast to seq_t for the final result            */
    seq_t odd = (seq_t)n * 2 + 1;
    return (seq_t)m - odd;
}

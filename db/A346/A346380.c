/*
 * A346380  Complement of A187430 in A000108.
 * https://oeis.org/A346380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the complement of A187430 in A000108. It is defined
 *            implicitly by the algebraic GF x*y^4 + 2*x^2*y^2 + x^3 + 3*x*y^2
 *            + y^3 - x*y = 0. The resulting sequence satisfies the recurrence
 *            a[0] = 0, a[1] = 0, for n ≥ 2: a[n] = C3[n+1] + C4[n] +
 *            2*C2[n-1] + δ_{n+1,3} + 3*C2[n], where C2[k] = Σ_{i...
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static idx_t maxComputed = 1;          /* last index that has been computed */
static size_t cap = 0;                 /* current allocated capacity */

static seq_t *A  = NULL; /* a[n]          */
static seq_t *C2 = NULL; /* conv2[n]      */
static seq_t *C3 = NULL; /* conv3[n]      */
static seq_t *C4 = NULL; /* conv4[n]      */

/* Ensure the arrays have at least 'need' capacity. */
static void ensure_capacity(idx_t need)
{
    if (cap <= (size_t)need) {
        size_t newCap = (cap == 0) ? (need + 4) : (cap * 2);
        if (newCap <= (size_t)need) newCap = need + 4;

        seq_t *newA  = (seq_t *)malloc(newCap * sizeof(seq_t));
        seq_t *newC2 = (seq_t *)malloc(newCap * sizeof(seq_t));
        seq_t *newC3 = (seq_t *)malloc(newCap * sizeof(seq_t));
        seq_t *newC4 = (seq_t *)malloc(newCap * sizeof(seq_t));

        if (A)  memcpy(newA,  A,  cap * sizeof(seq_t));
        if (C2) memcpy(newC2, C2, cap * sizeof(seq_t));
        if (C3) memcpy(newC3, C3, cap * sizeof(seq_t));
        if (C4) memcpy(newC4, C4, cap * sizeof(seq_t));

        if (A)  free(A);
        if (C2) free(C2);
        if (C3) free(C3);
        if (C4) free(C4);

        /* Zero out the newly allocated region */
        memset(newA  + cap, 0, (newCap - cap) * sizeof(seq_t));
        memset(newC2 + cap, 0, (newCap - cap) * sizeof(seq_t));
        memset(newC3 + cap, 0, (newCap - cap) * sizeof(seq_t));
        memset(newC4 + cap, 0, (newCap - cap) * sizeof(seq_t));

        A   = newA;
        C2  = newC2;
        C3  = newC3;
        C4  = newC4;
        cap = newCap;
    }
}

/* Initialise the arrays up to at least indices 1. */
static void init_once(void)
{
    if (cap == 0) {
        ensure_capacity(3);          /* need indices up to 2 at least */
        /* base values */
        A[0] = (seq_t)0;
        A[1] = (seq_t)0;
        C2[0] = (seq_t)0;
        C2[1] = (seq_t)0;
        C3[0] = (seq_t)0;
        C3[1] = (seq_t)0;
        C4[0] = (seq_t)0;
        C4[1] = (seq_t)0;
        /* C3[2] is needed for the next step but will be computed later */
        maxComputed = 1;
    }
}

seq_t a(idx_t n)
{
    init_once();

    if (n <= maxComputed)
        return A[n];

    /* Ensure enough storage for indices up to n+1 (for C3[n+1]) */
    ensure_capacity(n + 2);

    for (idx_t i = maxComputed + 1; i <= n; ++i) {
        /* C2[i] = sum_{j=1}^{i-1} a[j]*a[i-j] */
        seq_t sum2 = 0;
        for (idx_t j = 1; j < i; ++j)
            sum2 += A[j] * A[i - j];
        C2[i] = sum2;

        /* C3[i] = sum_{j=1}^{i-1} a[j]*C2[i-j] */
        seq_t sum3 = 0;
        for (idx_t j = 1; j < i; ++j)
            sum3 += A[j] * C2[i - j];
        C3[i] = sum3;

        /* C4[i] = sum_{j=1}^{i-1} a[j]*C3[i-j] */
        seq_t sum4 = 0;
        for (idx_t j = 1; j < i; ++j)
            sum4 += A[j] * C3[i - j];
        C4[i] = sum4;

        /* C3[i+1] = sum_{j=1}^{i} a[j]*C2[(i+1)-j] */
        seq_t sum3next = 0;
        for (idx_t j = 1; j <= i; ++j)
            sum3next += A[j] * C2[i + 1 - j];
        C3[i + 1] = sum3next;

        /* Compute a[i] from the recurrence */
        seq_t term1 = C3[i + 1];                           /* C3[n+1] */
        seq_t term2 = C4[i];                               /* C4[n]   */
        seq_t term3 = (seq_t)2 * C2[i - 1];                /* 2*C2[n-1] */
        seq_t term4 = ((i + 1) == 3) ? ((seq_t)1) : ((seq_t)0); /* δ_{n+1,3} */
        seq_t term5 = (seq_t)3 * C2[i];                     /* 3*C2[n]   */
        A[i] = term1 + term2 + term3 + term4 + term5;
    }

    maxComputed = n;
    return A[n];
}

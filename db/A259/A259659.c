/*
 * A259659  Expansion of phi(x^6) * f(-x)^3 / f(-x^3) in powers of x where
 *          phi(), f() are Ramanujan theta functions.
 * https://oeis.org/A259659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 524, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A259659
 *
 * Implementation: Euler transform of the period‑12 sequence
 * c(k) = [-3, -3, -2, -3, -3, -1, -3, -3, -2, -3, -3, -3]
 * The Euler transform gives
 *   a(0) = 1
 *   a(n) = (1/n) * Σ_{k=1..n} α(k) * a(n-k)
 *   α(k) = Σ_{d|k} d * c(d)
 *
 * The function is memoised and works for any order of calls.
 *
 * For C++ builds, std::vector is used for dynamic arrays.
 * For C builds, malloc/realloc is used; seq_t must be a built‑in type.
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Periodic sequence c(k) for k ≥ 1 (index starting at 0) */
static inline seq_t c_of(idx_t k) {
    static const seq_t c_period[12] = {
        -3, -3, -2, -3, -3, -1, -3, -3, -2, -3, -3, -3
    };
    return c_period[(k - 1) % 12];
}

/* Main function required by the judge */
seq_t a(idx_t n) {
    /* ---------- C++ implementation using std::vector ---------- */
#ifdef __cplusplus
    /* static storage for memoisation */
    static std::vector<seq_t> A;      /* a values   */
    static std::vector<seq_t> Alpha;  /* α values   */
    static size_t cap = 0;            /* number of stored values */

    /* ensure that the dynamic arrays are initialised */
    if (cap == 0) {
        A.resize(1);
        Alpha.resize(1);
        A[0] = 1;          /* a(0) = 1  */
        /* Alpha[0] is unused but initialise to 0 to be safe */
        Alpha[0] = 0;
        cap = 1;
    }

    /* ---- expand arrays if necessary ---- */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        if (newcap == 0) newcap = 1;
        while (newcap <= (size_t)n) newcap <<= 1;
        A.resize(newcap);
        Alpha.resize(newcap);

        /* compute α(k) for new indices using divisor sums */
        for (idx_t k = cap; k < (idx_t)newcap; ++k) {
            seq_t sum = 0;
            for (idx_t d = 1; d * d <= k; ++d) {
                if (k % d == 0) {
                    seq_t c1 = c_of(d);
                    sum += (seq_t)d * c1;
                    idx_t d2 = k / d;
                    if (d2 != d) {
                        seq_t c2 = c_of(d2);
                        sum += (seq_t)d2 * c2;
                    }
                }
            }
            Alpha[(size_t)k] = sum;
        }

        /* compute a(k) for new indices using the Euler recurrence */
        for (idx_t k = cap; k < (idx_t)newcap; ++k) {
            seq_t s = 0;
            for (idx_t m = 1; m <= k; ++m) {
                s += Alpha[(size_t)m] * A[(size_t)(k - m)];
            }
            A[(size_t)k] = s / (seq_t)k;   /* exact division */
        }

        cap = newcap;
    }

    return A[(size_t)n];
#else   /* ---------- plain C implementation ---------- */
    /* static storage for memoisation using malloc/realloc */
    static seq_t *A = NULL;
    static seq_t *Alpha = NULL;
    static size_t cap = 0;  /* number of stored values */

    /* initialise first entry a(0)=1 */
    if (cap == 0) {
        A = (seq_t *)malloc(sizeof(seq_t));
        Alpha = (seq_t *)malloc(sizeof(seq_t));
        if (!A || !Alpha) exit(EXIT_FAILURE);
        A[0] = 1;             /* a(0) = 1 */
        Alpha[0] = 0;         /* unused but keep zeroed */
        cap = 1;
    }

    /* expand dynamic arrays if required */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        if (newcap == 0) newcap = 1;
        while (newcap <= (size_t)n) newcap <<= 1;

        seq_t *tmpA = (seq_t *)realloc(A, newcap * sizeof(seq_t));
        seq_t *tmpAlpha = (seq_t *)realloc(Alpha, newcap * sizeof(seq_t));
        if (!tmpA || !tmpAlpha) exit(EXIT_FAILURE);
        A = tmpA;
        Alpha = tmpAlpha;

        /* compute α(k) for new indices */
        for (idx_t k = cap; k < (idx_t)newcap; ++k) {
            seq_t sum = 0;
            for (idx_t d = 1; d * d <= k; ++d) {
                if (k % d == 0) {
                    seq_t c1 = c_of(d);
                    sum += (seq_t)d * c1;
                    idx_t d2 = k / d;
                    if (d2 != d) {
                        seq_t c2 = c_of(d2);
                        sum += (seq_t)d2 * c2;
                    }
                }
            }
            Alpha[(size_t)k] = sum;
        }

        /* compute a(k) for new indices */
        for (idx_t k = cap; k < (idx_t)newcap; ++k) {
            seq_t s = 0;
            for (idx_t m = 1; m <= k; ++m) {
                s += Alpha[(size_t)m] * A[(size_t)(k - m)];
            }
            A[(size_t)k] = s / (seq_t)k;   /* exact division */
        }

        cap = newcap;
    }

    return A[(size_t)n];
#endif
}

/*
 * A307878  Expansion of e.g.f. exp(3*x)*(sec(x) + tan(x)).
 * https://oeis.org/A307878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A307878: a(n) = Σ_{k=0}^n C(n,k) * 3^{n-k} * E_k
 *   where E_k are the Euler zig numbers (coefficients of sec x + tan x).
 * The function computes the sequence using the recurrence for E_k:
 *   E_0 = 1, E_1 = 1, and for n ≥ 2,
 *   E_n = (1/2) * Σ_{k=0}^{n-1} C(n-1,k) * E_k * E_{n-1-k}.
 * The sequence is memoised so that any order of calls is efficient.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static buffers for Euler numbers, powers of 3, and the sequence itself. */
    static seq_t *E     = NULL;   /* Euler zig numbers E[0..computed] */
    static seq_t *pow3  = NULL;   /* powers 3^k up to computed   */
    static seq_t *aseq  = NULL;   /* a[n] values                */
    static idx_t  computed = 0;   /* highest n already computed */

    /* Initialise on first call. */
    if (E == NULL) {
        /* Allocate minimal arrays of size 1. */
        E     = (seq_t *)calloc(1, sizeof(seq_t));
        pow3  = (seq_t *)calloc(1, sizeof(seq_t));
        aseq  = (seq_t *)calloc(1, sizeof(seq_t));
        if (E == NULL || pow3 == NULL || aseq == NULL) {
            return (seq_t)0;
        }
        E[0]    = (seq_t)1;
        pow3[0] = (seq_t)1;
        aseq[0] = (seq_t)1;   /* a(0) = 1 */
        /* computed already 0 */
    }

    /* Return cached value if already computed. */
    if (n <= computed) {
        return aseq[(size_t)n];
    }

    /* Extend arrays up to n. */
    size_t newSize = (size_t)n + 1;
    E    = (seq_t *)realloc(E,    newSize * sizeof(seq_t));
    pow3 = (seq_t *)realloc(pow3, newSize * sizeof(seq_t));
    aseq = (seq_t *)realloc(aseq, newSize * sizeof(seq_t));
    if (E == NULL || pow3 == NULL || aseq == NULL) {
        return (seq_t)0;
    }

    /* Compute missing entries from computed+1 to n. */
    for (idx_t m = computed + 1; m <= n; ++m) {
        /* (1) Compute 3^m. */
        pow3[(size_t)m] = pow3[(size_t)(m - 1)] * (seq_t)3;

        /* (2) Compute Euler number E_m. */
        if (m == 1) {
            E[(size_t)m] = (seq_t)1;
        } else {
            seq_t eSum = (seq_t)0;
            seq_t curC = (seq_t)1;     /* C(m-1,0) */
            for (idx_t k = 0; k < m; ++k) {
                eSum += curC * E[(size_t)k] * E[(size_t)(m - 1 - k)];
                if (k + 1 < m) {
                    curC = curC * (seq_t)(m - 1 - k) / (seq_t)(k + 1);
                }
            }
            E[(size_t)m] = eSum / (seq_t)2;
        }

        /* (3) Compute a_m using the convolution. */
        seq_t aSum = (seq_t)0;
        seq_t curC = (seq_t)1;     /* C(m,0) */
        for (idx_t k = 0; k <= m; ++k) {
            aSum += curC * pow3[(size_t)(m - k)] * E[(size_t)k];
            if (k + 1 <= m) {
                curC = curC * (seq_t)(m - k) / (seq_t)(k + 1);
            }
        }
        aseq[(size_t)m] = aSum;
    }

    computed = n;
    return aseq[(size_t)n];
}

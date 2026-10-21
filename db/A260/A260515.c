/*
 * A260515  Expansion of phi(x^2) * chi(x)^4 in powers of x where phi(), chi()
 *          are Ramanujan theta functions.
 * https://oeis.org/A260515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 99, u64 n <= 355,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A260515: a(0)=1; for n>0, a(n) =
   (1/n) * Σ_{k=1..n} ( Σ_{d|k} d * c(d) ) * a(n‑k),
   where c(d) = 4,-2,4,-3,4,-2,4,-1 for d≡1,2,3,4,5,6,7,0 (mod 8). */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#if defined(__cplusplus)
#include <type_traits>
#endif

/* Periodic coefficients c(d).  Indexed by d % 8.  */
static const int8_t period[8] = { -1, 4, -2, 4, -3, 4, -2, 4 };

/*  Memoised data stored in static variables.  */
static seq_t *coeff      = NULL;   /* a(0)…a(k)   */
static size_t coeff_cap  = 0;      /* allocated size */
static int64_t *S        = NULL;   /* S(k) values */
static size_t S_cap      = 0;      /* allocated size */

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* --------------------------------------------------------------- */
    /*  Extend the memoised tables if necessary.  */
    /* --------------------------------------------------------------- */
    if ((size_t)n + 1 > coeff_cap) {
        size_t new_cap = (size_t)n + 1;
        size_t prev_cap = coeff_cap;

        /* Allocate / reallocate coefficient array.  */
        seq_t *new_coeff = (seq_t *)realloc(coeff, new_cap * sizeof(seq_t));
        if (!new_coeff) exit(EXIT_FAILURE);
        coeff = new_coeff;

        /* Allocate / reallocate S array.  */
        int64_t *new_S = (int64_t *)realloc(S, new_cap * sizeof(int64_t));
        if (!new_S) exit(EXIT_FAILURE);
        /* Initialize newly added cells.  */
        for (size_t i = prev_cap; i < new_cap; ++i) new_S[i] = 0;
        S = new_S;

        /* Update S for the new indices.  */
        for (size_t d = 1; d < new_cap; ++d) {
            int64_t add = (int64_t)d * period[d % 8];
            for (size_t m = d; m < new_cap; m += d) {
                if (m >= prev_cap) S[m] += add;
            }
        }

        /* Compute a(i) for i from prev_cap to new_cap-1.  */
        for (size_t i = prev_cap; i < new_cap; ++i) {
            if (i == 0) {                         /* a(0) = 1 */
                coeff[0] = (seq_t)1;
                continue;
            }

#if defined(__cplusplus)
            /* =========================================================== */
            /*  Big‑integer path (seq_t is a signed big‑int type).  */
            /* =========================================================== */
            if constexpr (sizeof(seq_t) <= 8) {    /* builtin integral types */
                /* ------------------------------------------------------ */
                /*  Small integer case: use 128‑bit intermediates.  */
                /* ------------------------------------------------------ */
                __int128 sum = 0;
                for (size_t k = 1; k <= i; ++k) {
                    sum += (__int128)coeff[i - k] * (__int128)S[k];
                }
                coeff[i] = (seq_t)(sum / (int)i);
            } else {                              /* arbitrary‑precision type */
                seq_t sum = 0;
                for (size_t k = 1; k <= i; ++k) {
                    sum += coeff[i - k] * (seq_t)S[k];
                }
                coeff[i] = sum / (seq_t)i;
            }
#else
            /* =========================================================== */
            /*  C build path: seq_t is a builtin integral type, use int128. */
            /* =========================================================== */
            __int128 sum = 0;
            for (size_t k = 1; k <= i; ++k) {
                sum += (__int128)coeff[i - k] * (__int128)S[k];
            }
            coeff[i] = (seq_t)(sum / (int)i);
#endif
        }

        coeff_cap = new_cap;
    }

    return coeff[n];
}

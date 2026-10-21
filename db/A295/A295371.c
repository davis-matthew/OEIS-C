/*
 * A295371  a(n) = (1/(2n))*Sum_{k=0..n-1} C(n-1, k)*C(n+k, k)*C(2k,
 *          k)*(k+2)*(-3)^(n-1-k).
 * https://oeis.org/A295371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A295371
 *
 * a(n) = (1/(2n)) * Σ_{k=0}^{n-1} C(n-1,k) * C(n+k,k) * C(2k,k) * (k+2) * (-3)^{n-1-k}
 *
 * Recurrence (for n ≥ 1):
 *   (2n+1) * (n+3)^2 * a(n+3) =
 *     (2n+1) * (7n^2 + 38n + 52) * a(n+2)
 *   + 3 * (2n+5) * (7n^2 + 4n + 1) * a(n+1)
 *   - 27 * (2n+5) * n^2 * a(n)
 *
 * Initial values:
 *   a(1) = 1
 *   a(2) = 3
 *   a(3) = 19
 *
 * The function below memoises computed terms and uses the recurrence for the
 * remaining values.
 */

#include <stdlib.h>

extern seq_t a(idx_t n);   /* forward declaration – supplied by the build */

static seq_t *CACHE = NULL;      /* array of a(0)…a(current_max) */
static idx_t  CACHE_SIZE = 0;    /* number of stored elements        */

/* Ensure that CACHE contains at least a(0)…a(n) */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= CACHE_SIZE)
        return;

    seq_t *new_cache = (seq_t *)realloc(CACHE, (n + 1) * sizeof(seq_t));
    if (!new_cache)
        abort();                       /* allocation failed */

    /* Copy the original pointer in case realloc moved the block. */
    CACHE = new_cache;

    for (idx_t i = CACHE_SIZE; i <= n; ++i)
    {
        if (i == 0) {                   /* sentinel for a(0), unused */
            CACHE[i] = 0;
            continue;
        }
        if (i == 1) {                   /* a(1) */
            CACHE[i] = 1;
            continue;
        }
        if (i == 2) {                   /* a(2) */
            CACHE[i] = 3;
            continue;
        }
        if (i == 3) {                   /* a(3) */
            CACHE[i] = 19;
            continue;
        }

        /* Use the recurrence for i ≥ 4.
         * Let m = i - 3 so that the recurrence concerns n = m. */
        idx_t m  = i - 3;
        seq_t a_nm2 = CACHE[i - 1];   /* a(m+2) = a(i-1) */
        seq_t a_nm1 = CACHE[i - 2];   /* a(m+1) = a(i-2) */
        seq_t a_n   = CACHE[i - 3];   /* a(m)   = a(i-3) */

        if (sizeof(seq_t) <= 8)         /* 64‑bit build: use __int128  */
        {
            __int128 m128    = m;
            __int128 two_m1  = 2 * m128 + 1;          /* 2n+1  */
            __int128 two_m5  = 2 * m128 + 5;          /* 2n+5  */
            __int128 m3      = m128 + 3;              /* n+3   */

            __int128 term1   = two_m1 *
                                ((__int128)7 * m128 * m128
                                 + (__int128)38 * m128 + (__int128)52);
            term1 *= (__int128)a_nm2;

            __int128 term2   = (__int128)3 * two_m5 *
                                ((__int128)7 * m128 * m128
                                 + (__int128)4 * m128 + (__int128)1);
            term2 *= (__int128)a_nm1;

            __int128 term3   = (__int128)27 * two_m5 * m128 * m128;
            term3 *= (__int128)a_n;

            __int128 numerator = term1 + term2 - term3;
            __int128 denom     = two_m1 * m3 * m3;     /* (2n+1)(n+3)^2 */

            CACHE[i] = (seq_t)(numerator / denom);    /* exact division */
        }
        else /* arbitrary‑precision build */
        {
            seq_t mm      = (seq_t)m;
            seq_t two_m1  = (seq_t)(2 * m + 1);   /* 2n+1  */
            seq_t two_m5  = (seq_t)(2 * m + 5);   /* 2n+5  */
            seq_t m3      = (seq_t)(m + 3);       /* n+3   */

            seq_t term1   = two_m1 *
                            ((seq_t)7 * mm * mm + (seq_t)38 * mm + (seq_t)52);
            term1 *= a_nm2;

            seq_t term2   = (seq_t)3 * two_m5 *
                            ((seq_t)7 * mm * mm + (seq_t)4 * mm + (seq_t)1);
            term2 *= a_nm1;

            seq_t term3   = (seq_t)27 * two_m5 * mm * mm;
            term3 *= a_n;

            seq_t numerator = term1 + term2 - term3;
            seq_t denom     = two_m1 * m3 * m3;     /* (2n+1)(n+3)^2 */

            CACHE[i] = numerator / denom;          /* exact division */
        }
    }

    CACHE_SIZE = n + 1;   /* now CACHE[0..n] are defined */
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* sequence is defined only for n ≥ 1 */
    ensure_cache(n);
    return CACHE[n];
}

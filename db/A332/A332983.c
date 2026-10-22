/*
 * A332983  Number of length-n binary words closed by an unbordered word.
 * https://oeis.org/A332983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{k=1}^{⌊n/2⌋} u(k) · f(n – 2k)
 *
 * where u(k) = number of binary unbordered words of length k,
 *     f(L) = number of binary words of length L that avoid a fixed
 *            unbordered pattern of length k.
 *
 * The values u(k) and f(L) are computed by the recurrences
 *
 *   u(1) = 2,
 *   u(k) = 2^k – Σ_{i=1}^{⌊k/2⌋} u(i) · 2^(k–2i)            (k ≥ 2)
 *
 *   f(0) = 1,
 *   f(L) = 2^L                          (0 < L < k),
 *   f(k) = 2^k – 1,
 *   f(L) = 2·f(L–1) – f(L–k)            (L > k)
 *
 * The implementation caches powers of two, the table u(k) and the
 * sequence a(n).  All indices are handled with unsigned arithmetic
 * and the code is safe for arbitrary‐precision seq_t as well as for
 * ordinary unsigned integer types.
 */

#include <stdlib.h>
#include <stddef.h>

/* --------- cache for powers of two ---------------------------------- */
static seq_t *pow2 = NULL;          /* 2^i                           */
static size_t pow2_len = 0;         /* current number of entries      */

static void ensure_pow2(idx_t n)
{
    if (n >= pow2_len)
    {
        size_t oldlen = pow2_len;
        size_t newlen = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(pow2, newlen * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failure */
        pow2 = tmp;
        if (oldlen == 0)            /* first initialization   */
        {
            pow2[0] = (seq_t)1;
            oldlen = 1;
        }
        for (size_t i = oldlen; i < newlen; ++i)
            pow2[i] = pow2[i-1] * (seq_t)2;
        pow2_len = newlen;
    }
}

/* --------- cache for unbordered word counts ------------------------ */
static seq_t *u_cache = NULL;      /* unbordered word counts         */
static size_t u_cache_len = 0;      /* current length                 */

static void ensure_u(idx_t max_k)
{
    if (max_k < (idx_t)1) return;   /* nothing to compute */

    /* ensure array big enough */
    if (max_k >= u_cache_len)
    {
        size_t newlen = (size_t)max_k + 1;
        if (newlen < 2) newlen = 2;          /* need indices 0 and 1       */
        seq_t *tmp = (seq_t*)realloc(u_cache, newlen * sizeof(seq_t));
        if (!tmp) exit(1);
        u_cache = tmp;
        size_t oldlen = u_cache_len;
        if (oldlen == 0)                      /* first initialization       */
        {
            u_cache[0] = (seq_t)0;
            u_cache[1] = (seq_t)2;
            oldlen = 2;
        }
        /* compute new entries */
        for (size_t k = oldlen; k <= max_k; ++k)
        {
            seq_t sum = (seq_t)0;
            for (size_t i = 1; i <= k/2; ++i)
                sum += u_cache[i] * pow2[k - 2*i];
            u_cache[k] = pow2[k] - sum;
        }
        u_cache_len = newlen;
    }
}

/* --------- temporary array for f(L) --------------------------------- */
static seq_t *f_temp = NULL;       /* temporary f(L) array           */
static size_t f_temp_len = 0;      /* its current allocated size     */

static void ensure_f_temp(idx_t max_len)
{
    /* max_len is guaranteed ≥ 1 when this function is used */
    if ((size_t)max_len + 1 > f_temp_len)
    {
        size_t newlen = (size_t)max_len + 1;
        seq_t *tmp = (seq_t*)realloc(f_temp, newlen * sizeof(seq_t));
        if (!tmp) exit(1);
        f_temp = tmp;
        f_temp_len = newlen;
    }
}

/* --------- cache for a(n) ------------------------------------------- */
static seq_t *a_cache = NULL;      /* a(n) values                    */
static size_t a_cache_len = 0;      /* current length                 */
static size_t cache_max_n = 0;      /* maximum n for which a_cache built */

static void recompute_a(idx_t max_n)
{
    if (max_n <= cache_max_n) return;          /* already built */

    /* only n ≥ 3 reach here; for n = 1,2 we return directly */
    ensure_pow2(max_n);
    ensure_u(max_n / 2);

    /* allocate a_cache */
    a_cache = (seq_t*)realloc(a_cache, (size_t)(max_n + 1) * sizeof(seq_t));
    if (!a_cache) exit(1);
    for (size_t i = 0; i <= (size_t)max_n; ++i) a_cache[i] = (seq_t)0;
    a_cache_len = max_n + 1;

    /* temporary array for f(L) */
    ensure_f_temp(max_n - 2);                  /* maximum possible Lmax */

    /* compute contributions for all k */
    for (size_t k = 1; k <= (size_t)(max_n / 2); ++k)
    {
        size_t Lmax = max_n - 2*k;
        seq_t *f = f_temp;                     /* reuse the same buffer */

        /* compute f(L) for this k */
        for (size_t L = 0; L <= Lmax; ++L)
        {
            if (L == 0)
                f[L] = (seq_t)1;
            else if (L < k)
                f[L] = pow2[L];
            else if (L == k)
                f[L] = pow2[k] - (seq_t)1;
            else
                f[L] = (seq_t)2 * f[L-1] - f[L-k];
        }

        seq_t uk = u_cache[k];
        for (size_t L = 0; L <= Lmax; ++L)
        {
            size_t idx = L + 2*k;              /* n = 2k + L */
            a_cache[idx] += uk * f[L];
        }
    }
    cache_max_n = max_n;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;       /* defined only for n ≥ 1 */
    if (n == (idx_t)1) return (seq_t)0;       /* a(1) = 0          */
    if (n == (idx_t)2) return (seq_t)2;       /* a(2) = 2          */

    if ((size_t)n > cache_max_n)
        recompute_a(n);
    return a_cache[(size_t)n];
}

/*
 * A325749  First term of n-th difference sequence of (floor(e*k/(e-1))), k >=
 *          0.
 * https://oeis.org/A325749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = first term of the n‑th finite difference sequence of *
 *            b(k) = ⌊e·k/(e-1)⌋, k ≥ 0
 * Range:     i8 n <= 1, i16 n <= 1, i32 none, i64 n <= 66, i128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* standard headers */
#include <stdint.h>
#include <stdlib.h>

/* forward declarations (if any helper functions are needed) */
static void init_constants(void);

/* ----------------------------------------------------------- */
/*  Constants used for the rational approximation of e/(e-1)   */
/* ----------------------------------------------------------- */
static seq_t const_num = 0;   /* numerator of the rational (≈ e)  */
static seq_t const_den = 0;   /* denominator of the rational      */
static int     is_inited  = 0;/* initialization flag              */
static unsigned long long fact20 = 0ULL;      /* 20!                          */

/* ----------------------------------------------------------- */
/*  Initialise constants (executed once)                       */
/* ----------------------------------------------------------- */
static void init_constants(void)
{
    if (is_inited) return;
    is_inited = 1;

    /* compute 20! */
    unsigned long long fact[21];
    fact[0] = 1ULL;
    for (unsigned i = 1; i <= 20; ++i) fact[i] = fact[i-1] * i;
    fact20 = fact[20];

    /* compute the numerator: Σ_{i=0}^{20} 20! / i! */
    unsigned long long sum = 0;
    for (unsigned i = 0; i <= 20; ++i)
        sum += fact20 / fact[i];

    const_num = (seq_t)sum;
    const_den = (seq_t)(sum - fact20);   /* = const_num - 20!  */
}

/* ----------------------------------------------------------- */
/*  Main sequence function                                      */
/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out of offset – not used */

    init_constants();

    /* allocate array for the n‑th difference computation */
    seq_t *buf = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!buf) return 0;             /* allocation failure – shouldn't happen */

    /* ----- Step 1: compute the base sequence f(k) = ⌊e·k/(e-1)⌋ ----- */
    /*      f(k) = ⌊ (const_num * k) / const_den ⌋                    */
    /* Note: const_num and const_den are small enough that 128‑bit
     * intermediate products are safe for idx_t up to at least 10⁷.     */
    for (idx_t k = 0; k <= n; ++k)
    {
        unsigned __int128 prod = (unsigned __int128)const_num * k;
        buf[k] = (seq_t)(prod / const_den);
    }

    /* ----- Step 2: iteratively compute differences ---------------- */
    idx_t len = n + 1;
    for (idx_t level = 1; level <= n; ++level)
    {
        for (idx_t i = 0; i < len - 1; ++i)
            buf[i] = buf[i+1] - buf[i];
        len--;   /* one element shorter after each difference level */
    }

    seq_t result = buf[0];
    free(buf);
    return result;
}

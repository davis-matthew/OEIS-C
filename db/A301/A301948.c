/*
 * A301948  Number of nX5 0..1 arrays with every element equal to 1, 2 or 3
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) * -29*a(n-2) * -114*a(n-3) * +324*a(n-4) *
 *            +8*a(n-5) * -709*a(n-6) * +690*a(n-7) * +158*a(n-8) *
 *            -448*a(n-9) * -34*a(n-10) * +116*a(n-11) * +27*a(n-12) *
 *            -4*a(n-13) for n > 15 * with initial values a(1)…a(15) as given
 *            below. The function memoises * the values so that any order
 *            of...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include only the headers required for malloc and size_t. */
#include <stdlib.h>

/* Sequence array with dynamic memoisation. */
static seq_t *seq = NULL;     /* memoised terms a(1), a(2), … */
static size_t memo_cap = 0;   /* size of `seq` allocation */
static size_t last_computed = 0;  /* number of terms already computed */

/* Base values a(1) … a(15).  Values are written exactly as in the OEIS
 * entry; use width that matches seq_t.  The compiler will convert them
 * as necessary.  They fit into 64 bits for the first 17 terms. */
static const seq_t base_values[15] = {
    (seq_t)3,
    (seq_t)52,
    (seq_t)680,
    (seq_t)9084,
    (seq_t)120815,
    (seq_t)1608681,
    (seq_t)21418808,
    (seq_t)285190208,
    (seq_t)3797277789,
    (seq_t)50560399640,
    (seq_t)673206912325,
    (seq_t)8963686254378,
    (seq_t)119350632636165,
    (seq_t)1589142359440249,
    (seq_t)21159279866124406
};

/*  Helper to ensure the cache is large enough.  The cache is reallocated
 *  only when a larger n is requested.  This function works for both C
 *  and C++pp17. */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n;
    if (need > memo_cap) {
        /* Reallocate: increase to maximum of current capacity and need. */
        seq_t *newp = (seq_t *)realloc(seq, need * sizeof(seq_t));
        if (!newp) abort();  /* In a real program we might handle this. */
        seq = newp;
        memo_cap = need;
    }
    /* Initialise any newly allocated space with zero; actual values
     * will be written when computations are performed. */
    if (last_computed < memo_cap) {
        for (size_t i = last_computed; i < memo_cap; ++i)
            seq[i] = (seq_t)0;
    }
}

/*  Compute a(n) using the recurrence.  The function is thread‐safe as
 *  long as the surrounding environment guarantees exclusive access.
 *  For simplicity, we assume single–threaded usage, which is typical
 *  for these exercise functions. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n < 1, but harmless */
    ensure_cache(n);
    /* Initialise base values if not already done. */
    if (last_computed < 15) {
        size_t upto = 15;
        if (n < upto) upto = (size_t)n;
        for (size_t i = 0; i < upto; ++i)
            seq[i] = base_values[i];
        last_computed = upto;
    }
    /* If we already have the answer, return it. */
    if (n <= last_computed)
        return seq[(size_t)n - 1];

    /* Compute terms a(16), a(17), … , a(n) successively. */
    for (size_t k = last_computed + 1; k <= (size_t)n; ++k) {
        /* Separate positive and negative contributions to avoid
         * underflow in unsigned arithmetic. */
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        /* Positive coefficients */
        pos += (seq_t)16  * seq[k-2];
        pos += (seq_t)324  * seq[k-5];
        pos += (seq_t)8    * seq[k-6];
        pos += (seq_t)690  * seq[k-8];
        pos += (seq_t)158  * seq[k-9];
        pos += (seq_t)116  * seq[k-12];
        pos += (seq_t)27   * seq[k-13];
        /* Negative coefficients (add their absolute values to neg) */
        neg += (seq_t)29  * seq[k-3];
        neg += (seq_t)114 * seq[k-4];
        neg += (seq_t)709 * seq[k-7];
        neg += (seq_t)448 * seq[k-10];
        neg += (seq_t)34  * seq[k-11];
        neg += (seq_t)4   * seq[k-14];
        /* The result is positive; subtracting a smaller unsigned value
         * is safe. */
        seq[k-1] = pos - neg;
    }
    last_computed = (size_t)n;
    return seq[(size_t)n - 1];
}

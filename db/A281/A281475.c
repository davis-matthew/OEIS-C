/*
 * A281475  Number of 7 X n 0..1 arrays with no element equal to more than two
 *          of its horizontal, diagonal or antidiagonal neighbors and with new
 *          values introduced in order 0 sequentially upwards.
 * https://oeis.org/A281475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-7) - a(n-8) for
 *            n > 16. The first 16 terms are derived from the rational
 *            generating function of the sequence. The function is memoised so
 *            that calls may arrive in any order
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 43, u64 n <= 97, u128 n <= 203,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static const int coeffs[16] = {
    64,       2657,   -4872,   2350,   -2775,   2258,
   -114,     -196,  -2915,   1992,   -209,    -31,
   -20,      -9,     -14,     -32
};

/* 1/D(x) where D(x)=1-2x+x^2-x^3+x^4-x^7+x^8  */
static seq_t inv_denom[16];
static bool inv_denom_computed = false;

static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* Compute the first 16 terms via convolution of the numerator
   with the power series 1/D(x).  All calculations are done with
   seq_t arithmetic to avoid overflow. */
static void compute_inv_denom(void)
{
    if (inv_denom_computed)
        return;

    inv_denom[0] = (seq_t)1;
    for (size_t i = 1; i < 16; ++i)
    {
        seq_t val = (seq_t)2 * inv_denom[i-1];
        if (i >= 2) val -= inv_denom[i-2];
        if (i >= 3) val += inv_denom[i-3];
        if (i >= 4) val -= inv_denom[i-4];
        if (i >= 7) val += inv_denom[i-7];
        if (i >= 8) val -= inv_denom[i-8];
        inv_denom[i] = val;
    }
    inv_denom_computed = true;
}

/* ensures seq_cache contains values up to index n (1‑based) */
static void ensure_seq_cache(idx_t n)
{
    if ((size_t)n <= seq_len)
        return;

    compute_inv_denom();

    if (seq_len == 0)
    {
        seq_cap = 16;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len  = 16;

        /* compute a(1)..a(16) */
        for (size_t i = 1; i <= 16; ++i)
        {
            seq_t pos = 0, neg = 0;
            size_t maxj = i;   /* i-j >=0 */
            for (size_t j = 1; j <= maxj; ++j)
            {
                int coeff = coeffs[j-1];
                seq_t term = inv_denom[i-j];
                if (coeff >= 0)
                    pos += (seq_t)coeff * term;
                else
                    neg += (seq_t)(-coeff) * term;
            }
            seq_cache[i-1] = pos - neg;
        }
    }

    /* extend beyond 16 using the empirical recurrence */
    while ((size_t)n > seq_len)
    {
        if (seq_len == seq_cap)
        {
            size_t new_cap = seq_cap ? seq_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            seq_cache = tmp;
            seq_cap = new_cap;
        }

        size_t i = seq_len + 1;   /* desired n (1‑based) */

        /* a(n) = 2*a(n-1) + a(n-3) + a(n-7) -
                 a(n-2) - a(n-4) - a(n-8)   */
        seq_t val = seq_cache[i-2];          /* a(n-1) */
        val += val;                          /* 2*a(n-1) */
        val += seq_cache[i-4];               /* + a(n-3) */
        val += seq_cache[i-8];               /* + a(n-7) */
        val -= seq_cache[i-3];               /* - a(n-2) */
        val -= seq_cache[i-5];               /* - a(n-4) */
        val -= seq_cache[i-9];               /* - a(n-8) */

        seq_cache[seq_len++] = val;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    ensure_seq_cache(n);
    return seq_cache[n-1];
}

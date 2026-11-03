/*
 * A371636  For any number k >= 0, let T_k be the triangle with values in {-1,
 *          0, +1} whose base corresponds to the balanced ternary expansion of
 *          k (without leading zeros) and other values, say t above u and v,
 *          satisfy t+u+v = 0 mod 3; this sequence lists the numbers k such
 *          that T_k has 3-fold rotational sym...
 * https://oeis.org/A371636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the nth integer k for which the triangle T_k built from the
 * balanced ternary expansion of k has 3‑fold rotational symmetry.
 * This implementation searches k in increasing order, building the triangle
 * for each k and testing rotational symmetry.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Convert a non‑negative integer k to its balanced ternary digits
   (msd first).  The digits are stored in array 'dig', length is returned.
   The array must be large enough to hold the digits (≤ 50). */
static size_t balanced_ternary(uint64_t k, int8_t *dig)
{
    if (k == 0)
    {
        dig[0] = 0;
        return 1;
    }
    size_t pos = 0;
    while (k > 0)
    {
        uint64_t r = k % 3;
        if (r == 0)
        {
            dig[pos++] = 0;
            k /= 3;
        }
        else if (r == 1)
        {
            dig[pos++] = 1;
            k /= 3;
        }
        else /* r == 2 */
        {
            dig[pos++] = -1;
            k = (k + 1) / 3;
        }
    }
    /* Reverse to get msd first */
    for (size_t i = 0; i < pos / 2; ++i)
    {
        int8_t tmp = dig[i];
        dig[i] = dig[pos - 1 - i];
        dig[pos - 1 - i] = tmp;
    }
    return pos;
}

/* Build triangle T_k and test for 3‑fold rotational symmetry.
   Returns true if symmetric, false otherwise. */
static bool is_symmetric(uint64_t k)
{
    /* Buffer for balanced digits (max 50). */
    int8_t digits[50];
    size_t m = balanced_ternary(k, digits);              /* number of digits */

    /* Triangle data: up to 50 rows, each row length ≤ 50. */
    int8_t tri[50][50] = {0};

    /* Base row (row 0). */
    for (size_t c = 0; c < m; ++c)
        tri[0][c] = digits[c];

    /* Build upper rows. */
    for (size_t r = 1; r < m; ++r)
    {
        for (size_t c = 0; c < m - r; ++c)
        {
            int sum = tri[r - 1][c] + tri[r - 1][c + 1];
            int rem = sum % 3;
            if (rem < 0) rem += 3;
            if (rem == 0)
                tri[r][c] = 0;
            else if (rem == 1)
                tri[r][c] = -1;
            else /* rem == 2 */
                tri[r][c] = 1;
        }
    }

    /* Test rotational symmetry. */
    for (size_t r = 0; r < m; ++r)
    {
        for (size_t c = 0; c < m - r; ++c)
        {
            size_t a = m - 1 - c - r;     /* a = m-1-c-r */
            size_t r2 = a;                /* rotated row */
            size_t c2 = r;                /* rotated column */
            if (tri[r][c] != tri[r2][c2])
                return false;
        }
    }
    return true;
}

/* Forward declarations of utility functions that may or may not be available */
#ifndef _COUNT
#define _COUNT
#endif

/* Function prototypes for static helpers */
static bool is_symmetric(uint64_t k);
static void extend_terms();

/* Memoization storage */
static seq_t *terms = NULL;          /* array of found terms */
static size_t terms_len = 0;         /* number of terms stored */
static size_t terms_cap = 0;         /* allocated capacity */
static uint64_t next_k = 0;          /* next k to test */

/* Ensure enough capacity for stored terms */
static void extend_terms()
{
    if (terms_len >= terms_cap)
    {
        size_t new_cap = (terms_cap == 0) ? 64 : terms_cap * 2;
        seq_t *new_terms = mem_new(seq_t, new_cap);
        if (!new_terms) return; /* allocation failure, give up */
        for (size_t i = 0; i < terms_len; ++i)
            new_terms[i] = terms[i];
        mem_free(terms);
        terms = new_terms;
        terms_cap = new_cap;
    }
}

/* Main sequence function as required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, but return 0 */

    /* Ensure zero‑based index: a(1) is first term. */
    size_t idx = (size_t)(n - 1);

    /* Generate terms until we have at least idx+1 terms. */
    while (terms_len <= idx)
    {
        if (is_symmetric(next_k))
        {
            extend_terms();
            terms[terms_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return terms[idx];
}

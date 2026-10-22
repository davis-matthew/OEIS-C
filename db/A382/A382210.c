/*
 * A382210  Irregular triangle read by rows: T(n,k) = k^2 - k + (A003173(n) +
 *          1)/4 with 1 <= k < (A003173(n) + 1)/4.
 * https://oeis.org/A382210
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy, fini, full, tabf
 * Formula:   irregular triangle read by rows, producing sequence A382210. For
 *            each prime p such that q = (p+1)/4 is also prime, the row has
 *            q-1 terms: T(k) = k^2 – k + q (1 ≤ k < q). The sequence a(n) is
 *            the concatenation of all rows, starting at a(4)=2
 * Range:     u8 n <= 35, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   mul_mod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mul_mod_u64.h"

/*------------------------------------------------------------------*/
/*  Auxiliary routines                                               */
/*------------------------------------------------------------------*/

/* Simple modular multiplication for 64‑bit values (used for primality). */

/* Simple modular exponentiation for 64‑bit values. */
static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1u;
    while (exp)
    {
        if (exp & 1u)
            result = mul_mod_u64(result, base, mod);
        base = mul_mod_u64(base, base, mod);
        exp >>= 1u;
    }
    return result;
}

/* Deterministic Miller–Rabin test for 64‑bit integers. */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2u) return false;
    static const uint64_t small[] =
        {2u,3u,5u,7u,11u,13u,17u,19u,23u,29u,31u,37u};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
    {
        if (n == small[i]) return true;
        if (n % small[i] == 0u) return false;
    }

    uint64_t d = n - 1u;
    unsigned int s = 0u;
    while ((d & 1u) == 0u)
    {
        d >>= 1u;
        ++s;
    }

    static const uint64_t bases[] =
        {2u, 325u, 9375u, 28178u, 450775u, 9780504u, 1795265022u};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0u) continue;
        uint64_t x = mod_pow_u64(a, d, n);
        if (x == 1u || x == n-1u) continue;
        bool composite = true;
        for (unsigned int r = 1u; r < s; ++r)
        {
            x = mul_mod_u64(x, x, n);
            if (x == n-1u)
            {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Grow a dynamic array of idx_t when needed. */
static void grow_idx_array(idx_t **arr, size_t *cap, size_t needed)
{
    if (*cap == 0u)
    {
        *cap = 8u;
        *arr = (idx_t*)malloc(*cap * sizeof(idx_t));
    }
    if (needed > *cap)
    {
        *cap <<= 1u;
        *arr = (idx_t*)realloc(*arr, *cap * sizeof(idx_t));
    }
}

/*------------------------------------------------------------------*/
/*  Main sequence routine                                           */
/*------------------------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n < 4u) return 0;

    idx_t offset = n - 4u;          /* 0‑based index in the concatenated rows */

    /*  Static state for generating rows on demand.  */
    static idx_t *q_vals = NULL;    /* q = (p+1)/4 for each found row */
    static idx_t *cum_terms = NULL;/* cumulative term count after each row */
    static size_t qcnt = 0u, qcap = 0u;
    static size_t cumcnt = 0u, cumcap = 0u;
    static idx_t next_q_candidate = 2u; /* next integer to try as q */

    /*  Generate rows until we cover the requested offset.  */
    while (cumcnt == 0u || cum_terms[cumcnt-1] <= offset)
    {
        /*  Find the next prime q.  */
        while (!is_prime_u64((uint64_t)next_q_candidate))
            ++next_q_candidate;
        idx_t q = next_q_candidate;
        ++next_q_candidate;                /* advance for next search */

        /*  p = 4q – 1 must also be prime.  */
        uint64_t p = 4u * (uint64_t)q - 1u;
        if (!is_prime_u64(p))
            continue;                     /* not a valid row */

        /*  Store q.  */
        grow_idx_array(&q_vals, &qcap, qcnt + 1u);
        q_vals[qcnt++] = q;

        /*  Prepare the cumulative count.  */
        idx_t row_len = q - 1u;           /* number of terms in this row */

        grow_idx_array(&cum_terms, &cumcap, cumcnt + 1u);
        if (cumcnt == 0u)
            cum_terms[0] = row_len;
        else
            cum_terms[cumcnt] = cum_terms[cumcnt-1] + row_len;
        ++cumcnt;
    }

    /*  Locate the row containing the requested offset.  */
    size_t row = 0u;
    while (row < cumcnt && cum_terms[row] <= offset)
        ++row;   /* first row with cum_terms[row] > offset */

    idx_t prev_cum = (row == 0u) ? 0u : cum_terms[row-1];
    idx_t k = offset - prev_cum + 1u;     /* 1‑based position within the row */

    /*  Compute the term: T(k) = k^2 – k + q.  */
    seq_t kk = (seq_t)k;
    seq_t result = kk * kk - kk + (seq_t)q_vals[row];
    return result;
}

/*
 * A330318  a(n) = Sum_{i=0..n} r(i)*r(i+1)/4, where r(n) = A004018(n) is the
 *          number of ways of writing n as a sum of two squares.
 * https://oeis.org/A330318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=0..n} r(i)*r(i+1)/4, where r(n) = A004018(n) =
 *            number of representations of n as a sum of two squares
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 61 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t r_calc(idx_t n)
{
    if (n == (idx_t)0)          /* by convention r(0)=1 */
        return (seq_t)1;

    idx_t m = n;
    seq_t prod = (seq_t)1;
    for (idx_t p = (idx_t)2; p * p <= m; ++p)
    {
        if (m % p == (idx_t)0)
        {
            int exp = 0;
            while (m % p == (idx_t)0)
            {
                m /= p;
                ++exp;
            }

            if (p % (idx_t)4 == (idx_t)1)          /* p ≡ 1 (mod 4) */
                prod *= (seq_t)(exp + 1);
            else if (p % (idx_t)4 == (idx_t)3 && (exp & 1) == 1)
                return (seq_t)0;                   /* p ≡ 3 (mod 4) with odd exponent */
        }
    }

    if (m > (idx_t)1)                            /* remaining prime factor > sqrt(n) */
    {
        if (m % (idx_t)4 == (idx_t)1)              /* prime ≡ 1 (mod 4) */
            prod *= (seq_t)2;
        else if (m % (idx_t)4 == (idx_t)3)         /* prime ≡ 3 (mod 4) */
            return (seq_t)0;                       /* odd exponent (1) */
        /* m == 2 or m ≡ 2 (mod 4) : no effect, prod stays the same */
    }

    return prod * (seq_t)4;                       /* final factor of 4 */
}

/*--- static cache for r(n) and a(n) -----------------------------------*/

static seq_t *R = NULL;      /* r(n) for n = 0 .. Rsize-1 */
static size_t Rsize = 0;

static seq_t *A = NULL;      /* a(n) for n = 0 .. Asize-1 */
static size_t Asize = 0;

/*--- Compute r(n) ------------------------------------------------------------------*/

/*--- Ensure r(0..k) is computed -----------------------------------------------*/

static void ensure_R(idx_t k)
{
    if (Rsize > (size_t)k) return;

    size_t newsize = (size_t)k + 1;
    R = mem_resize(seq_t, R, newsize);
    for (size_t i = Rsize; i < newsize; ++i)
        R[i] = r_calc((idx_t)i);

    Rsize = newsize;
}

/*--- Ensure a(0..n) is computed -----------------------------------------------*/

static void ensure_A(idx_t n)
{
    if (Asize > (size_t)n) return;

    size_t newsize = (size_t)n + 1;
    A = mem_resize(seq_t, A, newsize);

    seq_t sum = Asize > 0 ? A[Asize - 1] : (seq_t)0;

    for (size_t i = Asize; i < newsize; ++i)
    {
        idx_t idx = (idx_t)i;
        ensure_R(idx + (idx_t)1);                      /* need r(i+1) */
        seq_t term = R[idx] * R[idx + (idx_t)1] / (seq_t)4;
        sum += term;
        A[i] = sum;
    }

    Asize = newsize;
}

/*--- Public function -----------------------------------------------------------*/

seq_t a(idx_t n)
{
    ensure_A(n);
    return A[n];
}

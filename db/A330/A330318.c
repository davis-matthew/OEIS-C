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
 * Helpers:   r_calc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/r_calc.h"

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
    R = (seq_t *)realloc(R, newsize * sizeof(seq_t));
    for (size_t i = Rsize; i < newsize; ++i)
        R[i] = r_calc((idx_t)i);

    Rsize = newsize;
}

/*--- Ensure a(0..n) is computed -----------------------------------------------*/

static void ensure_A(idx_t n)
{
    if (Asize > (size_t)n) return;

    size_t newsize = (size_t)n + 1;
    A = (seq_t *)realloc(A, newsize * sizeof(seq_t));

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

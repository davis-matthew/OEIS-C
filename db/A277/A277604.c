/*
 * A277604  Array of coefficients T(k,n) of the formal power series A(k,x)
 *          read by upwards antidiagonals, where A(k,x) = sqrt(1 + 2*x*A(k,x)
 *          + (4*k+1)*x^2*(A(k,x))^2), k >= 0.
 * https://oeis.org/A277604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(k,n) read by upwards antidiagonals. * T(k,0) = 1. *
 *            T(k,2m+1) = (4k+1)^m. * T(k,2m) computed by recurrence: * T(k,0)
 *            = 1 * T(k,2m+2) = (4k+1) * T(k,2m) - 2 * k^(m+1) * Catalan(m) *
 *            (Catalan is A000108). The mapping from the linear index n to the
 *            * pair (k,ncol) is obtained by placing all...
 * Range:     u8 n <= 41, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   _floor_isqrt_u64, catalan, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"
#include "../utils/pow_int.h"
#include "../utils/_floor_isqrt_u64.h"

static seq_t compute_T(unsigned k, unsigned ncol)
{
    if (ncol == 0)
        return (seq_t)1;

    /* odd ncol: (4k+1)^(ncol/2) */
    if (ncol & 1) {
        seq_t base = (seq_t)4 * k + 1;
        return pow_int(base, ncol >> 1);
    }

    /* even ncol: iterate the recurrence */
    seq_t result = (seq_t)1;          /* T(k,0) */
    seq_t k_seq  = (seq_t)k;
    seq_t fourk1 = (seq_t)4 * k_seq + 1;
    seq_t k_power = k_seq;            /* k^(1) */

    unsigned m = ncol >> 1;           /* m such that ncol = 2*m */
    for (unsigned i = 0; i < m; ++i) {
        /* T(k,2*(i+1)) = (4k+1)*T(k,2i) - 2*k^(i+1)*Catalan(i) */
        seq_t mult = fourk1 * result;
        seq_t sub  = (seq_t)2 * k_power * catalan(i);
        result = mult - sub;

        /* prepare k^(i+2) for next iteration */
        if (i + 1 < m)
            k_power *= k_seq;
    }

    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find diagonal d such that d(d+1)/2 <= n < (d+1)(d+2)/2   */
    idx_t const five = 0; /* placeholder to avoid unused warning */
    idx_t const two  = 2;
    (void)five;          /* silence warning in C */

    /* compute d using integer sqrt: d = floor((sqrt(1+8n)-1)/2) */
    idx_t work = 8 * n + 1;
    idx_t s = _floor_isqrt_u64(work);
    idx_t d = (s - 1) / 2;

    /* triangular number up to previous diagonal */
    idx_t tri = d * (d + 1) / 2;
    idx_t offset = n - tri;

    /* map to (k, ncol) */
    unsigned k      = (unsigned)(d - offset);
    unsigned ncol   = (unsigned)offset;

    return compute_T(k, ncol);
}

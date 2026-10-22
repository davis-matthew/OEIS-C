/*
 * A355101  E.g.f. A(x) satisfies A(x) = 1 + 3 * x * A(exp(x) - 1).
 * https://oeis.org/A355101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A355101
   Formula: a(0)=1,
   a(n)=3*n*∑_{k=0}^{n-1} Stirling2(n-1,k)*a(k)   (n>0)    */

/* The implementation keeps two static arrays:
   - _a   : the computed a-values,
   - _s   : the current Stirling‑row S(n,k) for the last computed n.
   Both arrays grow as needed and are shared across calls of a().
*/

#include <stdlib.h>

/* Forward declarations of the private helpers */
static void _ensure_a_cap(size_t m);
static void _ensure_s_cap(size_t l);

/* Cache for the a-values */
static seq_t * _a      = NULL;   /* array of a-values               */
static size_t _a_cap   = 0;      /* allocated capacity              */
static size_t _last    = 0;      /* highest n that has been computed */

/* Cache for the current Stirling row S(_last,k)   */
static seq_t * _s      = NULL;   /* array holding S(_last,k) values  */
static size_t _s_cap   = 0;      /* allocated capacity for _s        */

/* Ensure that the a-array can hold indices up to m (inclusive). */
static void _ensure_a_cap(size_t m)
{
    if (_a_cap <= m) {
        size_t new_cap = (_a_cap < 8) ? 8 : _a_cap;
        while (new_cap <= m) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(_a, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        _a      = tmp;
        _a_cap  = new_cap;
    }
}

/* Ensure that the Stirling array can hold indices up to l-1. */
static void _ensure_s_cap(size_t l)
{
    if (_s_cap < l) {
        seq_t *tmp = (seq_t *)realloc(_s, l * sizeof(seq_t));
        if (!tmp) abort();
        _s      = tmp;
        _s_cap  = l;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* --- Initialization on first call ----------------------------------- */
    if (_a == NULL) {
        /* a(0) = 1 */
        _ensure_a_cap(0);
        _a[0] = (seq_t)1;
        _last = 0;

        /* Stirling row S(0,0) = 1 */
        _ensure_s_cap(1);
        _s[0]  = (seq_t)1;
    }

    /* Return the first term immediately if requested */
    if (n == 0) return (seq_t)1;

    /* --- Compute all terms up to n (if not already computed) ----------- */
    while (_last < n) {
        idx_t next = _last + 1;        /* index of the new term to compute */
        size_t len = (size_t)next;     /* number of elements in old Stirling row */

        /* Compute the inner sum: Σ_{k=0}^{next-1} S(next-1,k)*a(k) */
        seq_t sum = (seq_t)0;
        for (size_t k = 0; k < len; ++k) {
            sum += _s[k] * _a[k];
        }

        /* a(next) = 3 * next * sum */
        seq_t an = sum * (seq_t)next * (seq_t)3;

        /* Store the new a-value */
        _ensure_a_cap((size_t)next);
        _a[(size_t)next] = an;
        _last = next;                 /* update the highest computed index */

        /* Build the next Stirling row S(next,k), k=0..next */
        size_t new_len = len + 1;      /* new row has one more element */
        _ensure_s_cap(new_len);

        _s[new_len - 1] = (seq_t)1;    /* S(next,next) = 1 */
        for (size_t k = new_len - 1; k-- > 1; ) {
            _s[k] = (seq_t)k * _s[k] + _s[k - 1];
        }
        _s[0] = (seq_t)0;              /* S(next,0) = 0 */
    }

    /* Return the requested term */
    return _a[(size_t)n];
}

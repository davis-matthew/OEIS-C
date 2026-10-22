/*
 * A364377  The number of trailing 0's in the representation of n in
 *          Jacobsthal greedy base (A265747).
 * https://oeis.org/A364377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of trailing 0's in the greedy Jacobsthal base
 *            representation of n. The representation is obtained by
 *            repeatedly subtracting the largest Jacobsthal number ≤ the
 *            current remainder, allowing unlimited copies of each number
 * Range:     u8 n <= 388, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper */
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *J = NULL;          /* Jacobsthal numbers J1=1, J2=1, ...  */
static size_t Jlen = 0;          /* number of elements currently stored   */
static size_t Jcap = 0;          /* current capacity of the J array */

/* Ensure that J contains all Jacobsthal numbers ≤ n. */
static void
ensure_jacobsthal_upto(idx_t n)
{
    if (Jcap == 0) {                     /* first allocation          */
        Jcap = 16;
        Jlen = 0;
        J = seq_alloc(Jcap);
        J[0] = (seq_t)1;                 /* J1 */
        J[1] = (seq_t)1;                 /* J2 */
        Jlen = 2;
    }
    /* Grow the table until the largest stored element exceeds n. */
    while ((seq_t)n >= J[Jlen - 1]) {
        if (Jlen == Jcap) {               /* double capacity if needed */
            Jcap *= 2;
            J = seq_realloc(J, Jlen, Jcap);
        }
        /* Recurrence: Jk = J(k-1) + 2*J(k-2)   (k >= 3) */
        seq_t next = J[Jlen - 1] + ((seq_t)2 * J[Jlen - 2]);
        J[Jlen] = next;
        ++Jlen;
    }
}

/* Return the largest index i (0‑based, J1 at 0) with J[i] ≤ val. */
static idx_t
largest_jacobsthal_le(seq_t val)
{
    for (idx_t i = Jlen; i-- > 0; ) {
        if (J[i] <= val)
            return i;
    }
    return (idx_t)-1;   /* never reached for val > 0 */
}

seq_t a(idx_t n)
{
    if (n == 0)           /* offset 1; n=0 is irrelevant            */
        return (seq_t)0;

    ensure_jacobsthal_upto(n);

    seq_t  rem = (seq_t)n;
    idx_t  min_idx = Jlen;          /* larger than any valid index  */

    /* Greedy decomposition.  Recording the smallest index used. */
    while (rem > 0) {
        idx_t j = largest_jacobsthal_le(rem);   /* j is 0‑based index  */
        if (j < min_idx)
            min_idx = j;
        rem -= J[j];
    }

    /* a(n) = max(0, min_idx - 1)   because J1 has index 0.   */
    if (min_idx <= 1)
        return (seq_t)0;
    else
        return (seq_t)(min_idx - 1);
}

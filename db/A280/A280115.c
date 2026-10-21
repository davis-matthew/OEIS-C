/*
 * A280115  Partial products of A175317 (Sum_{d|n} pod(d)).
 * https://oeis.org/A280115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=1..n} Σ_{d|i} pod(d) where pod(d) = product of
 *            divisors of d. The algorithm computes pod(d) directly by
 *            multiplying all divisors, then accumulates the partial products.
 *            All values are stored in static arrays so any call order is
 *            handled
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   pod_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pod_of.h"

/* static tables */
static seq_t *pod_arr = NULL;   /* pod values, indices 1..n */
static idx_t pod_cap = 0;

static seq_t *a_arr   = NULL;   /* sequence a(n), indices 1..n */
static idx_t a_cap   = 0;
static idx_t a_last  = 0;        /* largest n computed so far */

/* --------------------------------------------------------------------- */
/*  Compute pod(m) = product of all divisors of m                        */

/* --------------------------------------------------------------------- */
/*  Ensure pod_arr contains pod(1..n)                                   */
/* --------------------------------------------------------------------- */
static void
ensure_pod(idx_t n)
{
    if (n <= pod_cap)
        return;
    seq_t *new_pod = (seq_t *)realloc(pod_arr, (n + 1) * sizeof(seq_t));
    if (!new_pod && n > 0)
        abort();              /* allocation failure */
    pod_arr = new_pod;
    for (idx_t i = pod_cap + 1; i <= n; ++i)
        pod_arr[i] = pod_of(i);
    pod_cap = n;
}

/* --------------------------------------------------------------------- */
/*  Sum of pod(d) over all divisors d of n                              */
/* --------------------------------------------------------------------- */
static seq_t
divisor_pod_sum(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            sum += pod_arr[d];
            idx_t other = n / d;
            if (other != d)
                sum += pod_arr[other];
        }
    }
    return sum;
}

/* --------------------------------------------------------------------- */
/*  Public function a(n):  compute the nth value of the sequence         */
/* --------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0)   /* offset is 1; behaviour below offset not required */
        abort();

    /* Prepare the pod table up to n */
    ensure_pod(n);

    /* Allocate or enlarge the a_arr array if needed */
    if (a_arr == NULL) {
        a_arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!a_arr)
            abort();
        a_cap = n;
        a_arr[1] = 1;           /* a(1) = 1 */
        a_last  = 1;
    } else if (n > a_cap) {
        seq_t *tmp = (seq_t *)realloc(a_arr, (n + 1) * sizeof(seq_t));
        if (!tmp)
            abort();
        a_arr = tmp;
        a_cap = n;
    }

    /* Compute missing terms up to n */
    for (idx_t i = a_last + 1; i <= n; ++i) {
        seq_t sum = divisor_pod_sum(i);
        a_arr[i] = a_arr[i - 1] * sum;
    }
    a_last = n;
    return a_arr[n];
}

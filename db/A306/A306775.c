/*
 * A306775  Partial sums of A060648: sum of the inverse Moebius transform of
 *          the Dedekind psi function from 1 to n.
 * https://oeis.org/A306775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} 2^ω(k) * floor(n/k) * floor(1 + n/k) / 2
 *            where ω(k) is the number of distinct prime factors of k. This is
 *            equivalent to the OEIS formula a(n) = Sum_{k=1..n} Sum_{d|k}
 *            μ^2(d)
 * Range:     u8 n <= 14, u16 n <= 229, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static idx_t   max_cached = 0;     /* largest n for which tables were built */
static idx_t   *omega      = NULL; /* number of distinct prime factors */
static seq_t   *pow2omega  = NULL; /* 2^ω(k) for 1 <= k <= max_cached */

/* Build tables up to n (inclusive).  If n is already <= max_cached,
   the tables are unchanged. */
static void build_tables(idx_t n)
{
    if (n <= max_cached)
        return;

    /* Reallocate space for new size.  realloc is safe for both C """
    /* and C++ because seq_t is a basic unsigned integer type or a type */
    /* with a trivial constructor (as required by the build). */
    omega = (idx_t *)realloc(omega, (n + 1) * sizeof(idx_t));
    pow2omega = (seq_t *)realloc(pow2omega, (n + 1) * sizeof(seq_t));
    if (!omega || !pow2omega)
        return; /* Allocation failed – caller will return 0. */

    /* Clear all entries before sieving. */
    for (idx_t i = 0; i <= n; ++i)
        omega[i] = 0;

    /* Simple distinct‑prime sieve. */
    for (idx_t p = 2; p <= n; ++p) {
        if (omega[p] == 0) {           /* p is prime */
            for (idx_t m = p; m <= n; m += p)
                ++omega[m];
        }
    }

    /* Pre‑compute 2^ω(k).  pow2omega[0] is unused but set for safety. */
    pow2omega[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow2omega[i] = ((seq_t)1 << omega[i]);

    max_cached = n;
}

seq_t a(idx_t n)
{
    if (n == 0)  /* offset starts at 1; by definition a(0)=0 */
        return (seq_t)0;

    build_tables(n);

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t ak   = (seq_t)(n / k);          /* floor(n/k)                 */
        seq_t term = ak * (ak + 1) / 2;       /* floor(n/k)*(floor+1)/2     */
        sum += pow2omega[k] * term;           /* 2^ω(k) * term              */
    }
    return sum;
}

/*
 * A318750  a(n) = Sum_{k=1..n} k * tau_3(k), where tau_3 is A007425.
 * https://oeis.org/A318750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k * tau_3(k) where tau_3(k) = Sum_{d|k}
 *            tau(d), tau(d) = number of divisors of d. The implementation
 *            recomputes the divisor tables and cumulative sums from scratch
 *            whenever a larger n is requested, which is fast enough for n up
 *            to ~10 000
 * Range:     u8 n <= 8, u16 n <= 89, u32 n <= 8654, u64 n <= 8654,
 *            u128 n <= 8654, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t max_n = 0;                    /* maximum n that has been computed   */
static uint32_t *tau  = NULL;              /* ordinary divisor counts            */
static uint32_t *tau3 = NULL;              /* 3‑divisor counts                  */
static seq_t    *cum  = NULL;              /* cumulative sums: cum[n] = a(n)     */

/* Recompute all tables up to n (inclusive).  All tables are allocated to
   size n+1 and zero‑initialized. */
static void
ensure_capacity(idx_t n)
{
    if (n <= max_n) return;               /* nothing to do */

    /* free old tables if they exist (calloc below will overwrite) */
    if (tau)  free(tau);
    if (tau3) free(tau3);
    if (cum)  free(cum);

    /* allocate new tables */
    tau  = (uint32_t *)calloc((size_t)(n + 1), sizeof(uint32_t));
    tau3 = (uint32_t *)calloc((size_t)(n + 1), sizeof(uint32_t));
    cum  = (seq_t    *)calloc((size_t)(n + 1), sizeof(seq_t));

    /* compute tau: number of divisors for every integer <= n */
    for (idx_t d = 1; d <= n; ++d) {
        for (idx_t j = d; j <= n; j += d)
            ++tau[j];
    }

    /* compute tau3: Sum of tau(d) over divisors d of each number */
    for (idx_t d = 1; d <= n; ++d) {
        uint32_t td = tau[d];
        for (idx_t j = d; j <= n; j += d)
            tau3[j] += td;
    }

    /* compute cumulative sums a(n) */
    cum[0] = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t term = (seq_t)i * (seq_t)tau3[i];
        cum[i] = cum[i - 1] + term;
    }

    max_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* offset is 1; a(0) is unused */

    ensure_capacity(n);
    return cum[n];
}

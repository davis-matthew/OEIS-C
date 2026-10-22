/*
 * A381747  a(n) is the number of solutions to tau(x) + tau(n-x) = tau(n)
 *          where 1 <= x <= floor(n/2).
 * https://oeis.org/A381747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ 1 ≤ x ≤ ⌊n/2⌋ | τ(x)+τ(n−x)=τ(n) }
 * Range:     u8 n <= 3662, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif

/* ------------------------------------------------------------
   Divisor–count cache τ[0…max]
   ------------------------------------------------------------ */
#if defined(__cplusplus)
/* In C++ we use a vector to store the divisor counts. */
static std::vector<seq_t> tau_arr;
/* -----------------------------------------------------------------
   Recompute τ[1…max] wiping old values.
   ----------------------------------------------------------------- */
static void recompute_taus(idx_t max)
{
    size_t maxu = (size_t)max;
    /* initialise all entries to zero */
    tau_arr.assign(maxu + 1, 0);

    /* sieve‑like accumulation of divisor counts */
    for (size_t i = 1; i <= maxu; ++i) {
        for (size_t j = i; j <= maxu; j += i)
            ++tau_arr[j];
    }
}
#else
/* In plain C we use a raw dynamic array. */
static seq_t *tau_arr = NULL;
static size_t tau_size = 0;
/* -----------------------------------------------------------------
   Recompute τ[1…max], allocating a new zeroed array.
   ----------------------------------------------------------------- */
static void recompute_taus(idx_t max)
{
    size_t maxu = (size_t)max;
    /* allocate and zero the array */
    seq_t *new_tau = calloc(maxu + 1, sizeof(seq_t));
    if (!new_tau) abort();  /* out of memory */
    free(tau_arr);
    tau_arr  = new_tau;
    tau_size = maxu + 1;

    /* sieve‑like accumulation of divisor counts */
    for (size_t i = 1; i <= maxu; ++i) {
        for (size_t j = i; j <= maxu; j += i)
            ++tau_arr[j];
    }
}
#endif

/* -----------------------------------------------------------------
   Ensure τ[0…max] are available, recomputing if necessary.
   ----------------------------------------------------------------- */
static void ensure_taus(idx_t max)
{
#if defined(__cplusplus)
    if (tau_arr.size() <= (size_t)max)
        recompute_taus(max);
#else
    if ((size_t)max + 1 > tau_size)
        recompute_taus(max);
#endif
}

/* ------------------------------------------------------------
   Main OEIS sequence function
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* out of range – no term defined */

    ensure_taus(n);              /* make sure τ[0…n] are computed */

    seq_t result = 0;
    idx_t half   = n >> 1;       /* floor(n/2) */

    seq_t tau_n  = tau_arr[(size_t)n];   /* τ(n) from the cache */

    for (idx_t x = 1; x <= half; ++x) {
        seq_t tau_x  = tau_arr[(size_t)x];
        seq_t tau_nx = tau_arr[(size_t)(n - x)];
        if (tau_x + tau_nx == tau_n)
            ++result;
    }

    return result;
}

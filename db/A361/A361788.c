/*
 * A361788  Number of divisors of n that are totient values (A002202).
 * https://oeis.org/A361788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A361788:
 *  a(n) = number of divisors of n that are values of Euler's totient function φ(m).
 *
 *  The algorithm:
 *  1. Pre‑compute φ(k) for 0 <= k <= LIMIT and record which numbers appear as φ.
 *  2. For each requested n factor it using the supplied helper factor_u64.
 *  3. Generate all divisors of n from its prime factorisation.
 *  4. Count those divisors that are marked as a totient value.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Utility: factorisation of 64‑bit integers. */
#include "../utils/factor_u64.h"

#define LIMIT 1000000U            /* enough for n up to ~10^4 and beyond */

static uint32_t phi_arr[LIMIT + 1];
static bool   is_phi[LIMIT + 1];
static bool   phi_initialized = false;

/* ------------------------------------------------------------ */
/*  Pre‑compute φ(k) for k <= LIMIT and the set of totient values. */
/* ------------------------------------------------------------ */
static void init_totients(void)
{
    if (phi_initialized)
        return;

    /* initialise φ[i] = i */
    for (uint32_t i = 0; i <= LIMIT; ++i)
        phi_arr[i] = i;

    /* sieve to compute φ */
    for (uint32_t p = 2; p <= LIMIT; ++p) {
        if (phi_arr[p] == p) {             /* p is prime */
            for (uint32_t j = p; j <= LIMIT; j += p)
                phi_arr[j] -= phi_arr[j] / p;
        }
    }

    /* mark all numbers that occur as φ(m) for some m */
    memset(is_phi, 0, sizeof(is_phi));
    for (uint32_t i = 1; i <= LIMIT; ++i) {
        uint32_t val = phi_arr[i];
        if (val <= LIMIT)
            is_phi[val] = true;
    }
    phi_initialized = true;
}

/* ------------------------------------------------------------ */
/*  Main function requested by the build system.                 */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_totients();

    if (n == 0ULL)                 /* sequence is defined for n >= 1 */
        return (seq_t)0;

    /* factor n into primes and exponents */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t   prime_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &prime_cnt);

    /* generate divisors in a small fixed array (max 240 for n <= 1e6) */
    uint64_t divs[256];
    size_t   divs_len = 1;
    divs[0] = 1ULL;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        size_t   current_len = divs_len;

        for (size_t j = 0; j < current_len; ++j) {
            uint64_t base = divs[j];
            uint64_t mult = base;
            for (uint64_t k = 1; k <= e; ++k) {
                mult *= p;
                divs[divs_len++] = mult;
            }
        }
    }

    /* count divisors that are totient values */
    uint64_t count = 0;
    for (size_t i = 0; i < divs_len; ++i) {
        uint64_t d = divs[i];
        bool is_tot = false;
        if (d == 1ULL)              /* φ(1) = 1 */
            is_tot = true;
        else if ((d & 1ULL) == 0ULL)  /* only even divisors >1 can be totients */
            is_tot = d <= LIMIT && is_phi[(uint32_t)d];
        if (is_tot)
            ++count;
    }

    return (seq_t)count;
}

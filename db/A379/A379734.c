/*
 * A379734  Number of integer partitions of n into parts > 1 whose product is
 *          a multiple of n.
 * https://oeis.org/A379734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 59, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   compute_multipliers
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Sequence A379734
   Formula: a(n) = number of integer partitions of n into parts > 1 whose product is a multiple of n
*/

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/compute_multipliers.h"

/* ------------------------------------------------------------ */
/*  Helper: factorise n into primes and exponents                */
/* ------------------------------------------------------------ */
static void factorize(idx_t n, uint64_t *primes, uint64_t *exps, size_t *k)
{
    *k = 0;
    uint64_t x = n;
    for (uint64_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            uint64_t cnt = 0;
            while (x % p == 0)
            {
                x /= p;
                ++cnt;
            }
            primes[(*k)] = p;
            exps[(*k)]   = cnt;
            ++(*k);
        }
    }
    if (x > 1)
    {
        primes[(*k)] = x;
        exps[(*k)]   = 1;
        ++(*k);
    }
}

/* ------------------------------------------------------------ */
/*  Helper: compute mixed base multipliers for the state space */

/* ------------------------------------------------------------ */
/*  Main sequence function                                     */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* a(1) = 0 */

    /* -------- factorise n --------------------------------- */
    uint64_t primes[16], exps[16];
    size_t np;
    factorize(n, primes, exps, &np);

    /* -------- number of states (mixed radix) -------------- */
    size_t mult[16];
    compute_multipliers(exps, np, mult);

    /* total number of states = product (exp_i + 1)           */
    size_t states = 1;
    for (size_t i = 0; i < np; ++i)
        states *= (exps[i] + 1);

    /* target state (all exponents reached)                   */
    size_t target = 0;
    for (size_t i = 0; i < np; ++i)
        target += exps[i] * mult[i];

    /* -------- pre‑compute digit tables for each state ------ */
    uint8_t *digits = (uint8_t *)malloc(states * np * sizeof(uint8_t));
    if (!digits) return (seq_t)0;

    for (size_t idx = 0; idx < states; ++idx)
    {
        for (size_t i = 0; i < np; ++i)
            digits[idx * np + i] = (uint8_t)((idx / mult[i]) % (exps[i] + 1));
    }

    /* -------- buffer for the mapping of a state when a part is added */
    size_t *new_map = (size_t *)malloc(states * sizeof(size_t));
    if (!new_map) { free(digits); return (seq_t)0; }

    /* -------- DP array : dp[sum * states + state] ------------ */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1) * states, sizeof(seq_t));
    if (!dp) { free(digits); free(new_map); return (seq_t)0; }

    dp[0 * states + 0] = (seq_t)1;         /* empty partition */

    /* -------- outer loop over part sizes -------------------- */
    for (idx_t s = 2; s <= n; ++s)
    {
        /* exponents of s relative to the primes of n               */
        uint8_t e[16] = {0};
        uint64_t tmp   = (uint64_t)s;
        for (size_t i = 0; i < np; ++i)
        {
            while (tmp % primes[i] == 0 && e[i] < exps[i])
            {
                ++e[i];
                tmp /= primes[i];
            }
        }

        /* pre‑compute mapping of every state after adding part s */
        for (size_t idx = 0; idx < states; ++idx)
        {
            size_t new_idx = 0;
            for (size_t i = 0; i < np; ++i)
            {
                uint8_t val = digits[idx * np + i];
                uint8_t add = e[i];
                if (val + add > exps[i]) add = exps[i] - val;
                new_idx += ((size_t)(val + add)) * mult[i];
            }
            new_map[idx] = new_idx;
        }

        /* update DP table for this part size                          */
        for (idx_t sum = s; sum <= n; ++sum)
        {
            size_t base      = (size_t)sum * states;
            size_t prev_base = (size_t)(sum - s) * states;
            for (size_t idx = 0; idx < states; ++idx)
            {
                seq_t val = dp[prev_base + idx];
                if (val != (seq_t)0)
                    dp[base + new_map[idx]] += val;
            }
        }
    }

    /* -------- result ------------------------------------- */
    seq_t result = dp[(size_t)n * states + target];

    free(dp);
    free(digits);
    free(new_map);
    return result;
}

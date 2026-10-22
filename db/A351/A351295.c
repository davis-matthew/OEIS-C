/*
 * A351295  Numbers whose multiset of prime factors has no permutation with
 *          all distinct run-lengths.
 * https://oeis.org/A351295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A351295: Numbers whose multiset of prime factors has no
 * permutation with all distinct run‑lengths.
 *
 * The function probes integers successively and tests whether a
 * suitable assignment of distinct run‑lengths exists.  If such an
 * assignment exists, the integer is **not** part of the sequence;
 * otherwise it is.  The test is performed by recursive backtracking
 * that builds the distinct partitions for each exponent while
 * maintaining a bitmask of run‑lengths already used globally.
 *
 * Complexity is benign for the first ten thousand terms (roughly
 * O(number of candidates · number of exponents · partition branching)
 * which stays far below the 2‑second limit. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* ------------------------------------------------------------ */
/*  Recursive search for a valid assignment of run‑lengths.     */
/* ------------------------------------------------------------ */

static bool _search(int step,
                    uint64_t used_mask,
                    const int *exps,
                    size_t *order,
                    size_t k);

/* Generate all distinct partitions of 'rem' using parts <= 'max_part'.
 * Each part is added to used_mask; once 'rem' reaches zero, we
 * recurse to the next exponent.                                                      */
static bool _gen_parts(int rem,
                       int max_part,
                       uint64_t used_mask,
                       int exp_val,
                       int step,
                       const int *exps,
                       size_t *order,
                       size_t k)
{
    if (rem == 0)                      /* completed this exponent */
        return _search(step + 1, used_mask, exps, order, k);

    for (int part = max_part; part >= 1; --part) {
        uint64_t bit = 1ULL << part;
        if (used_mask & bit)           /* run‑length already used */
            continue;
        /* recurse with this part chosen */
        if (_gen_parts(rem - part,
                       part - 1,
                       used_mask | bit,
                       exp_val,
                       step,
                       exps,
                       order,
                       k))
            return true;
    }
    return false;
}

static bool _search(int step,
                    uint64_t used_mask,
                    const int *exps,
                    size_t *order,
                    size_t k)
{
    if (step == k)                     /* all exponents processed */
        return true;

    int exp_val = exps[order[step]];
    return _gen_parts(exp_val, exp_val, used_mask,
                     exp_val, step, exps, order, k);
}

/* ------------------------------------------------------------ */
/*  Test whether integer m can be represented with all distinct  */
/*  run‑lengths.  Returns true if such an assignment exists, i.e.  */
/*  the integer is NOT in the sequence.                           */
/* ------------------------------------------------------------ */
static bool _has_valid_assignment(uint64_t m)
{
    if (m < 2)               /* 1 has no prime factors */
        return false;

    /* Factorisation.  We use 64‑bit arrays; enough for any 64‑bit m. */
    uint64_t primes[64], exps64[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps64, &cnt);
    if (cnt == 0)
        return false;        /* should not happen for m>=2 */

    /* Convert exponents to int and prepare ordering.  Sort by descending exponent. */
    int exps[64];
    size_t order[64];
    for (size_t i = 0; i < cnt; ++i) {
        exps[i] = (int)exps64[i];
        order[i] = i;
    }
    /* Simple bubble sort: larger exponents first. */
    for (size_t i = 0; i < cnt; ++i)
        for (size_t j = i + 1; j < cnt; ++j)
            if (exps[order[i]] < exps[order[j]]) {
                size_t t = order[i];
                order[i] = order[j];
                order[j] = t;
            }

    /* Search for any valid assignment. */
    return _search(0, 0ULL, exps, order, cnt);
}

/* ------------------------------------------------------------ */
/*  Public sequence function                                     */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined, but safe for callers */

    idx_t found = 0;
    uint64_t m = 2;        /* start from the smallest integer >1 */

    while (1) {
        if (!_has_valid_assignment(m)) {
            ++found;
            if (found == n)
                return (seq_t)m;
        }
        ++m;
    }
}

/*
 * A338605  Resistance values R < 1 ohm, multiplied by their common
 *          denominator 840 (= A338600(5)), that can be obtained from a
 *          network of exactly 5 one-ohm resistors, but not from any network
 *          with fewer than 5 one-ohm resistors.
 * https://oeis.org/A338605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   cmp_seq_t, gcd, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A1 fixed resistor network resistance sequence A338605  
 *  Returns a(n) = 840 * p/q, where p/q is a resistance value < 1 Ω    
 *  that can be realized with exactly five 1‑Ω resistors,  
 *  but not with any fewer.  
 *  (The sequence is sorted increasingly;  a(1) = 168, a(2) = 240, …)   */

/*  Implementation details:  
 *  - All fractions are kept in lowest terms using gcd.  
 *  - For k from 1 to 5 we build the set of all resistances reachable with  
 *    exactly k resistors by combining 1‑to‑(k‑1) network with a 1‑to‑(k‑(k‑1))  
 *    network in series and parallel.  
 *  - After the five‑resistor sets are ready, we filter out resistances that  
 *    were already achievable with ≤ 4 resistors.  
 *  - Remaining fractions < 1 are converted to integers 840·p/q and stored.  
 *  - Results are sorted once; subsequent calls just index the array.   */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/cmp_seq_t.h"
#include "../utils/seq_alloc.h"

typedef struct { uint64_t num, den; } Fraction;

/* Reduce a fraction to lowest terms using gcd. */
static Fraction reduce_frac(Fraction f)
{
    seq_t g = gcd((seq_t)f.num, (seq_t)f.den);
    Fraction r = { f.num / (uint64_t)g, f.den / (uint64_t)g };
    return r;
}

/* Add a fraction to an array if it is not already present. */
static void add_to_set(Fraction *set, size_t *sz, const Fraction *v)
{
    for (size_t i = 0; i < *sz; ++i)
        if (set[i].num == v->num && set[i].den == v->den)
            return;
    set[(*sz)++] = *v;
}

/* Test whether a fraction is in an array. */
static bool in_set(const Fraction *set, size_t sz, const Fraction *v)
{
    for (size_t i = 0; i < sz; ++i)
        if (set[i].num == v->num && set[i].den == v->den)
            return true;
    return false;
}

/* Series connection: a + b. */
static Fraction series(const Fraction *a, const Fraction *b)
{
    __uint128_t num = (__uint128_t)a->num * b->den + (__uint128_t)b->num * a->den;
    __uint128_t den = (__uint128_t)a->den * b->den;
    Fraction r = { (uint64_t)num, (uint64_t)den };
    return reduce_frac(r);
}

/* Parallel connection: 1/(1/a + 1/b). */
static Fraction parallel(const Fraction *a, const Fraction *b)
{
    __uint128_t num = (__uint128_t)a->num * b->num;
    __uint128_t den = (__uint128_t)a->num * b->den + (__uint128_t)b->num * a->den;
    Fraction r = { (uint64_t)num, (uint64_t)den };
    return reduce_frac(r);
}

seq_t a(idx_t n)
{
    static seq_t *arr = NULL;
    static size_t arr_len = 0;

    if (n < 1)
        return 0;

    /* Initialise only once. */
    if (!arr)
    {
        /* Fractions reachable with exactly k resistors. */
        Fraction set_exact[6][256];
        size_t sz_exact[6] = {0};

        /* k = 1: a single resistor gives 1 Ω. */
        set_exact[1][0] = (Fraction){1, 1};
        sz_exact[1] = 1;

        /* Build sets for k = 2 … 5. */
        for (int k = 2; k <= 5; ++k)
        {
            for (int i = 1; i <= k - 1; ++i)
            {
                int j = k - i;
                for (size_t ai = 0; ai < sz_exact[i]; ++ai)
                {
                    for (size_t bi = 0; bi < sz_exact[j]; ++bi)
                    {
                        Fraction res;

                        /* Series. */
                        res = series(&set_exact[i][ai], &set_exact[j][bi]);
                        if (840 % res.den == 0)
                            add_to_set(set_exact[k], &sz_exact[k], &res);

                        /* Parallel. */
                        res = parallel(&set_exact[i][ai], &set_exact[j][bi]);
                        if (840 % res.den == 0)
                            add_to_set(set_exact[k], &sz_exact[k], &res);
                    }
                }
            }
        }

        /* Union of fractions achievable with ≤ 4 resistors. */
        Fraction union_set[256];
        size_t sz_union = 0;
        for (int k = 1; k <= 4; ++k)
            for (size_t i = 0; i < sz_exact[k]; ++i)
                if (!in_set(union_set, sz_union, &set_exact[k][i]))
                    union_set[sz_union++] = set_exact[k][i];

        /* Keep fractions from exactly 5 resistors that are < 1 and not in union_set. */
        Fraction final_set[256];
        size_t sz_final = 0;
        for (size_t i = 0; i < sz_exact[5]; ++i)
        {
            const Fraction *f = &set_exact[5][i];
            if (f->num >= f->den)          /* R ≥ 1 Ω -> discard */
                continue;
            if (!in_set(union_set, sz_union, f))
                final_set[sz_final++] = *f;
        }

        /* Allocate array of seq_t (supports arbitrary‑precision). */
        arr = seq_alloc(sz_final);
        for (size_t i = 0; i < sz_final; ++i)
        {
            __uint128_t prod = (__uint128_t)840 * final_set[i].num;
            arr[i] = (seq_t)(prod / final_set[i].den);
        }
        arr_len = sz_final;

        /* Sort the results. */
        qsort(arr, arr_len, sizeof(seq_t), cmp_seq_t);
    }

    return (n - 1 < arr_len) ? arr[n - 1] : (seq_t)0;
}

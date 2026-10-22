/*
 * A361568  Expansion of e.g.f. exp(x^3/6 * (1+x)^3).
 * https://oeis.org/A361568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} binomial(3*k,n-3*k)/(6^k * k!)
 *            Recurrence (valid for n>=3): a(0)=1; a(1)=0; a(2)=0;
 *            a(n)=((n-1)*(n-2)/6) *
 *            (3*a(n-3)+12*(n-3)*a(n-4)+15*(n-3)*(n-4)*a(n-5)+6*(n-3)*(n-4)*(n-5)*a(n-6))
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* ==== Memoisation cache ==== */
static seq_t *cache = NULL;    /* holds a(0)…a(curr-1)  */
static size_t curr  = 0;       /* next index to compute   */
static size_t cap   = 0;       /* allocated array size    */

/* Grow cache so that it can hold indices up to n (inclusive). */
static void ensure_size(idx_t n)
{
    if ((size_t)n + 1 <= cap) return;

    size_t new_cap = (size_t)n + 1;
    if (new_cap == 0) new_cap = 8;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) return;            /* allocation failure ignored – caller will fail */

    cache = new_cache;
    cap   = new_cap;
}

/* ---- main sequence function ---- */
seq_t a(idx_t n)
{
    ensure_size(n);

    /* compute missing values up to n */
    while (curr <= n)
    {
        if (curr == 0)
            cache[curr] = 1;
        else if (curr == 1 || curr == 2)
            cache[curr] = 0;
        else
        {
            seq_t sum = 0;

            if (curr >= 3)
                sum += (seq_t)3 * cache[curr - 3];
            if (curr >= 4)
                sum += (seq_t)12 * (seq_t)(curr - 3) * cache[curr - 4];
            if (curr >= 5)
                sum += (seq_t)15 * (seq_t)(curr - 3) *
                       (seq_t)(curr - 4) * cache[curr - 5];
            if (curr >= 6)
                sum += (seq_t)6 * (seq_t)(curr - 3) *
                       (seq_t)(curr - 4) * (seq_t)(curr - 5) *
                       cache[curr - 6];

            seq_t factor = (seq_t)(curr - 1) * (seq_t)(curr - 2);

            /* use 128‑bit arithmetic to avoid overflow in 64‑bit builds   */
            __uint128_t prod = ( (__uint128_t)factor ) * ( (__uint128_t)sum );
            prod /= 6;
            cache[curr] = (seq_t)prod;
        }

        curr++;
    }

    return cache[n];
}

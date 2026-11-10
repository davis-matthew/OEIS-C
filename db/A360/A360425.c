/*
 * A360425  Indices of records in A018804.
 * https://oeis.org/A360425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the n-th record of A018804 (Pillai function P(m)
 *            = Sum_{k=1..m} gcd(k,m), multiplicative, P(p^e) =
 *            p^(e-1)*((e+1)p-e)). Computed by a search: build the Pareto
 *            front (records) of p-smooth numbers prime by prime; the final
 *            front below a limit gives the record indices
 * Range:     u8 n <= 38, u16 n <= 122, u32 n <= 286, u64 n <= 286,
 *            u128 n <= 286, big any n
 * Validated: standard, pass, 76 terms through n = 262 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 rec_u128;

typedef struct { rec_u128 n; rec_u128 p; } rec_pair;

static int rec_cmp(const void *x, const void *y)
{
    const rec_pair *a = (const rec_pair *)x;
    const rec_pair *b = (const rec_pair *)y;
    if (a->n < b->n) return -1;
    if (a->n > b->n) return 1;
    return 0;
}

static rec_u128 *rec_terms = 0;
static size_t rec_count = 0;
static rec_u128 rec_limit = 0;

static void rec_build(rec_u128 X)
{
    static const unsigned primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67,
        71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139,
        149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223,
        227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293,
        307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383,
        389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
        467, 479, 487, 491, 499
    };
    const size_t np = sizeof(primes) / sizeof(primes[0]);

    size_t fcnt = 1, fcap = 16;
    rec_pair *front = (rec_pair *)malloc(fcap * sizeof(rec_pair));
    front[0].n = 1; front[0].p = 1;

    for (size_t k = 0; k < np; ++k)
    {
        rec_u128 q = primes[k];
        if (q > X) break;
        size_t ccnt = 0, ccap = 64;
        rec_pair *cand = (rec_pair *)malloc(ccap * sizeof(rec_pair));
        for (size_t i = 0; i < fcnt; ++i)
        {
            rec_u128 n = front[i].n, pv = front[i].p;
            rec_u128 qpow = 1; /* q^(e-1) */
            unsigned e = 0;
            for (;;)
            {
                if (ccnt == ccap)
                {
                    ccap *= 2;
                    cand = (rec_pair *)realloc(cand, ccap * sizeof(rec_pair));
                }
                cand[ccnt].n = n;
                if (e == 0) cand[ccnt].p = pv;
                else cand[ccnt].p = pv * (qpow * ((rec_u128)(e + 1) * q - e));
                ++ccnt;
                if (n > X / q) break;
                n *= q;
                ++e;
                if (e >= 2) qpow *= q;
                /* recompute multiplier at push time using e */
                if (e == 1) qpow = 1;
            }
        }
        qsort(cand, ccnt, sizeof(rec_pair), rec_cmp);
        fcnt = 0;
        rec_u128 best = 0;
        for (size_t i = 0; i < ccnt; ++i)
        {
            if (cand[i].p > best)
            {
                best = cand[i].p;
                if (fcnt == fcap)
                {
                    fcap *= 2;
                    front = (rec_pair *)realloc(front, fcap * sizeof(rec_pair));
                }
                front[fcnt++] = cand[i];
            }
        }
        free(cand);
    }

    free(rec_terms);
    rec_terms = (rec_u128 *)malloc((fcnt + 1) * sizeof(rec_u128));
    for (size_t i = 0; i < fcnt; ++i) rec_terms[i] = front[i].n;
    rec_count = fcnt;
    rec_limit = X;
    free(front);
}

static seq_t rec_to_seq(rec_u128 v)
{
    uint64_t hi = (uint64_t)(v >> 64);
    uint64_t lo = (uint64_t)v;
    if (hi == 0) return (seq_t)lo;
    return (seq_t)hi * (seq_t)4294967296u * (seq_t)4294967296u + (seq_t)lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    rec_u128 cap = (rec_u128)1;
    for (int i = 0; i < 34; ++i) cap *= 10;
    while (rec_count < (size_t)n)
    {
        rec_u128 X;
        if (rec_limit == 0) X = 1000000;
        else X = rec_limit * 1000;
        if (rec_limit >= cap) return (seq_t)0;
        if (X > cap) X = cap;
        rec_build(X);
    }
    return rec_to_seq(rec_terms[n - 1]);
}

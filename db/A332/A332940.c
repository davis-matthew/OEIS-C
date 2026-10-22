/*
 * A332940  a(n) is the least prime p such that p = x^2 + (2g+1)*y^2 and p !=
 *          2*g+1 and p mod (2g+1) != 1 where g is the n-th Sophie Germain
 *          prime.
 * https://oeis.org/A332940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that * p = x² + (2g+1)·y², * p ≠ 2g+1
 *            and p mod (2g+1) ≠ 1, * where g is the n‑th Sophie Germain prime
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   floor_sqrt, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/isPrimeGeneric.h"
#include "../utils/floor_sqrt.h"

/* -------------------------------------------------------------------------- */
/* Storage for Sophie Germain primes (1‑based index in sg_primes[n‑1])       */
/* -------------------------------------------------------------------------- */
static uint64_t *sg_primes = NULL;
static size_t    sg_cap    = 0;
static size_t    sg_count  = 0;
static uint64_t  sg_last   = 1;   /* last integer that was tested */

/* -------------------------------------------------------------------------- */
static void ensure_sg_primes(idx_t n)
{
    if (sg_count >= (size_t)n) return;

    while (sg_count < (size_t)n)
    {
        uint64_t cand = sg_last + 1;   /* next integer to test */
        sg_last = cand;

        if (!isPrimeGeneric((seq_t)cand))            /* cand must be prime   */
            continue;

        uint64_t two_plus = 2 * cand + 1;           /* 2·cand+1 must also be prime */
        if (!isPrimeGeneric((seq_t)two_plus))
            continue;

        if (sg_count >= sg_cap)                    /* enlarge storage      */
        {
            size_t new_cap = sg_cap ? sg_cap * 2 : 16;
            uint64_t *tmp = (uint64_t *)realloc(sg_primes,
                                                new_cap * sizeof(uint64_t));
            if (!tmp) abort();
            sg_primes = tmp;
            sg_cap = new_cap;
        }

        sg_primes[sg_count++] = cand;              /* store the SG prime */
    }
}

/* -------------------------------------------------------------------------- */
/* Find the least prime satisfying the conditions for a given p0 = 2g+1.      */
/* -------------------------------------------------------------------------- */
static seq_t find_min_prime(seq_t p0)
{
    uint64_t cand = (uint64_t)p0 + 2;              /* first odd > p0+1 */
    if ((cand & 1u) == 0) cand++;

    for (;;)
    {
        /* only trial the prime candidates            */
        if (!isPrimeGeneric((seq_t)cand)) { cand += 2; continue; }

        seq_t cand_seq = (seq_t)cand;

        /* reject if remainder == 1                         */
        if (cand_seq % p0 == (seq_t)1) { cand += 2; continue; }

        /* test representation x² + p0·y²  */
        for (uint64_t y = 0; ; ++y)
        {
            seq_t y_seq = (seq_t)y;
            seq_t y2    = y_seq * y_seq;
            seq_t sub   = p0 * y2;
            if (sub > cand_seq) break;

            seq_t x2 = cand_seq - sub;
            seq_t s  = floor_sqrt(x2);
            if (s * s == x2) return cand_seq;    /* found the minimal prime */
        }

        cand += 2;                                /* try next odd prime   */
    }
}

/* -------------------------------------------------------------------------- */
/* Cache for computed sequence terms                                        */
/* -------------------------------------------------------------------------- */
static seq_t *seq_a = NULL;
static size_t a_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for index 0 */

    /* enlarge cache if needed */
    if (a_cap <= (size_t)n)
    {
        size_t new_cap = a_cap ? a_cap * 2 : 16;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;

        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) abort();

        if (seq_a)
        {
            memcpy(tmp, seq_a, a_cap * sizeof(seq_t));
            free(seq_a);
        }

        memset(tmp + a_cap, 0, (new_cap - a_cap) * sizeof(seq_t));
        seq_a = tmp;
        a_cap = new_cap;
    }

    /* return cached value if already computed */
    if (seq_a[n] != 0) return seq_a[n];

    /* compute nth Sophie Germain prime g */
    ensure_sg_primes(n);
    uint64_t g = sg_primes[n - 1];
    seq_t p0 = (seq_t)(2 * (seq_t)g + (seq_t)1);

    /* find the minimal prime meeting the conditions */
    seq_t ans = find_min_prime(p0);

    seq_a[n] = ans;
    return ans;
}

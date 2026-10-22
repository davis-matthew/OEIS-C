/*
 * A308140  Sum of the largest side lengths of all integer-sided triangles
 *          with squarefree side lengths and perimeter n.
 * https://oeis.org/A308140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{k=1..floor(n/3)} Sum_{i=k..floor((n-k)/2)}
 *            sign(floor((i+k)/(n-i-k+1))) * mu(i)^2 * mu(k)^2 * mu(n-i-k)^2 *
 *            (n-i-k) (A308140: largest side of square‑free integer triangles)
 * Range:     u8 n <= 44, u16 n <= 310, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes.h"   /* provides build_primes()          */

/* static storage for square‑free flags and prime list */
static unsigned char *sqfree = NULL;
static idx_t sqfree_limit = 0;
static idx_t *primes = NULL;
static size_t primes_count = 0;

/* build square‑free table up to limit (inclusive) */
static void ensure_sqfree(idx_t limit)
{
    if (limit <= sqfree_limit)
        return;

    unsigned char *new_sqfree = (unsigned char *)malloc((size_t)(limit + 1));
    if (!new_sqfree)
        return;                      /* out of memory – abort silently */

    /* initialise as all square‑free (except 0) */
    new_sqfree[0] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        new_sqfree[i] = 1;

    /* get primes up to limit */
    idx_t *new_primes = NULL;
    size_t new_pcount = 0;
    build_primes(limit, &new_primes, &new_pcount);

    /* mark multiples of p² as non‑square‑free */
    for (size_t pi = 0; pi < new_pcount; ++pi) {
        idx_t p = new_primes[pi];
        idx_t p2 = p * p;
        if (p2 > limit)
            break;
        for (idx_t j = p2; j <= limit; j += p2)
            new_sqfree[j] = 0;
    }

    /* replace old arrays */
    free(primes);
    free(sqfree);
    primes = new_primes;
    primes_count = new_pcount;
    sqfree = new_sqfree;
    sqfree_limit = limit;
}

/* ------------------------------------------------------------ */
/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                       /* no triangle possible */

    ensure_sqfree(n);

    seq_t sum = 0;

    idx_t start_c = (n + 2) / 3;         /* ceil(n/3)   */
    idx_t end_c   = (n - 1) / 2;         /* floor((n‑1)/2) */

    if (end_c < start_c)
        return 0;

    for (idx_t c = start_c; c <= end_c; ++c) {
        if (!sqfree[c])
            continue;

        /* compute valid a‑range */
        idx_t low;
        if (n <= 2 * c)
            low = 1;
        else
            low = n - 2 * c;           /* n - 2c  (guaranteed ≥1) */

        idx_t high = (n - c) / 2;       /* floor((n‑c)/2) */

        if (high < low)
            continue;

        for (idx_t a = low; a <= high; ++a) {
            if (!sqfree[a])
                continue;
            idx_t b = n - c - a;       /* derived from perimeter */
            if (!sqfree[b])
                continue;

            /* a <= b <= c and triangle inequality hold automatically */
            sum += (seq_t)c;
        }
    }
    return sum;
}

/*
 * A262785  Number of ordered ways to write n as x^2 + y^2 + p*(p+d)/2, where
 *          0 <= x <= y, d is 1 or -1, and p is prime.
 * https://oeis.org/A262785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered ways to write n as x^2 + y^2 +
 *            p*(p+d)/2 with 0 ≤ x ≤ y, d ∈ {−1, +1}, and p prime
 * Range:     u8 n <= 35, u16 n <= 2726, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sqrt_floor.h"
#include "../utils/build_primes.h"

/* Cached prime table */
static idx_t *prime_cache = NULL;
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    /* free old allocation if any */
    free(prime_cache);
    size_t cnt;
    idx_t *arr = NULL;
    /* build_primes: primes ≤ limit */
    build_primes(limit, &arr, &cnt);
    prime_cache = arr;
    prime_cnt   = cnt;
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0)           /* sequence offset starts at 1 */
        return 0;

    seq_t count = 0;
    seq_t nsq   = (seq_t)n;

    /* Largest prime p with p*(p‑1)/2 ≤ n */
    idx_t maxp = 1;
    while (true) {
        idx_t nxt = maxp + 1;
        seq_t t   = (seq_t)nxt * (nxt - 1) / 2;
        if (t <= nsq)
            maxp = nxt;
        else
            break;
    }
    if (maxp < 2)
        maxp = 2;

    /* Build or extend prime list */
    ensure_primes(maxp);

    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = (seq_t)prime_cache[i];

        /* d = –1  →  p*(p‑1)/2 */
        seq_t t1 = p * (p - 1) / 2;
        if (t1 <= nsq) {
            seq_t rem = nsq - t1;
            idx_t  maxx = (idx_t)sqrt_floor(rem);
            for (idx_t x = 0; x <= maxx; ++x) {
                seq_t xsq = (seq_t)x * x;
                seq_t y2  = rem - xsq;
                seq_t y   = sqrt_floor(y2);
                if (y * y == y2 && y >= x)
                    ++count;
            }
        }

        /* d = +1  →  p*(p+1)/2 */
        seq_t t2 = p * (p + 1) / 2;
        if (t2 <= nsq) {
            seq_t rem = nsq - t2;
            idx_t  maxx = (idx_t)sqrt_floor(rem);
            for (idx_t x = 0; x <= maxx; ++x) {
                seq_t xsq = (seq_t)x * x;
                seq_t y2  = rem - xsq;
                seq_t y   = sqrt_floor(y2);
                if (y * y == y2 && y >= x)
                    ++count;
            }
        }
    }

    return count;
}

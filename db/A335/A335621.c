/*
 * A335621  Number of integer-sided triangles with perimeter n such that the
 *          sum of each pair of side lengths is squarefree.
 * https://oeis.org/A335621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer‑sided triangles with perimeter n * such
 *            that each of the three pairwise sums is squarefree
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* static cache for Mobius square‑free information */
static int8_t  *mu           = NULL; /* Mobius values: -1, 0, 1 */
static uint8_t *sqfree_arr   = NULL; /* 1 if square‑free, 0 otherwise */
static uint8_t *is_composite_arr = NULL;
static uint32_t *primes_arr    = NULL;
static size_t  mu_size   = 0; /* current maximum n for which mu is built */

/* ensure sqfree_arr is valid up to limit (inclusive) */
static void ensure_squarefree(size_t limit)
{
    if (mu_size >= limit) return;            /* already available */

    /* enlarge all needed arrays */
    mu = (int8_t*)realloc(mu,   (limit + 1) * sizeof(int8_t));
    sqfree_arr = (uint8_t*)realloc(sqfree_arr,
                                   (limit + 1) * sizeof(uint8_t));
    is_composite_arr = (uint8_t*)realloc(is_composite_arr,
                                         (limit + 1) * sizeof(uint8_t));
    primes_arr = (uint32_t*)realloc(primes_arr,
                                    limit * sizeof(uint32_t));

    memset(is_composite_arr, 0, (limit + 1) * sizeof(uint8_t));

    mu[0] = 0;
    mu[1] = 1;
    size_t primes_cnt = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (!is_composite_arr[i]) {
            primes_arr[primes_cnt++] = (uint32_t)i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p  = primes_arr[j];
            size_t    v = i * (size_t)p;
            if (v > limit) break;
            is_composite_arr[v] = 1;
            if (i % p == 0) {
                mu[v] = 0;
                break;
            } else {
                mu[v] = (int8_t)(-mu[i]);
            }
        }
    }

    for (size_t i = 0; i <= limit; ++i)
        sqfree_arr[i] = (mu[i] != 0) ? 1 : 0;

    mu_size = limit;
}

/* public function implementing the sequence */
seq_t a(idx_t n)
{
    if (n < 3) return 0;        /* no triangle possible */

    ensure_squarefree((size_t)n); /* need square‑free up to n */

    seq_t count = 0;

    for (idx_t a = 1; a <= n / 3; ++a) {
        idx_t b_max = (n - a) / 2;
        for (idx_t b = a; b <= b_max; ++b) {
            idx_t c = n - a - b;
            if (a + b <= c) continue;         /* not a triangle */

            idx_t sum_ab = a + b;
            idx_t sum_ac = a + c;
            idx_t sum_bc = b + c;

            if (!sqfree_arr[sum_ab] ||
                !sqfree_arr[sum_ac] ||
                !sqfree_arr[sum_bc]) continue;

            count += (seq_t)1;
        }
    }
    return count;
}

/*
 * A328016  Numbers k such that k, k+1, ... k+6 are all cubefree (A004709).
 * https://oeis.org/A328016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 5539, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer k such that k, k+1, … , k+6 are all cubefree
   (OEIS A328016).  The algorithm sieves smallest prime factors up to a
   fixed limit and tests each candidate k≡1 (mod 8) for the cubefree
   property.  This is a straightforward search rather than a closed
   form. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LIMIT 5000000U   /* sufficient for n ≤ 10 000 */

static uint32_t *spf = NULL;   /* smallest prime factor array */

/* Initialise the sieve of smallest prime factors up to MAX_LIMIT. */
static void init_spf(void)
{
    if (spf != NULL) return;               /* already initialised */

    spf = (uint32_t *)calloc(MAX_LIMIT + 1, sizeof(uint32_t));
    if (spf == NULL) exit(EXIT_FAILURE);

    for (uint32_t i = 2; i <= MAX_LIMIT; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = i;
            if ((uint64_t)i * i <= MAX_LIMIT) {
                for (uint32_t j = i * i; j <= MAX_LIMIT; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }
}

/* Test whether x is cubefree using the precomputed spf array.  x must be
   ≤ MAX_LIMIT. */
static bool is_cubefree(uint64_t x)
{
    if (x <= 1) return true;
    while (x > 1) {
        uint32_t p = spf[x];
        uint8_t  cnt = 0;
        while (x % p == 0) {
            x /= p;
            ++cnt;
            if (cnt >= 3) return false;
        }
    }
    return true;
}

/* Return the n‑th term of A328016. */
seq_t a(idx_t n)
{
    init_spf();

    idx_t count = 0;
    for (idx_t k = 1; k + 6 <= MAX_LIMIT; ++k) {
        if ((k & 7) != 1) continue;      /* only k ≡ 1 (mod 8) */

        bool ok = true;
        for (uint8_t off = 0; off < 7; ++off) {
            if (!is_cubefree((uint64_t)(k + off))) {
                ok = false;
                break;
            }
        }
        if (!ok) continue;

        ++count;
        if (count == n) return (seq_t)k;
    }

    /* If the bound is exceeded, return 0 as a sentinel (should not
       happen for the intended range). */
    return (seq_t)0;
}

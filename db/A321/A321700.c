/*
 * A321700  Base-5 deletable primes (written in base 10).
 * https://oeis.org/A321700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th prime that is deletable in base 5. Compute it
 *            by scanning successive primes and testing the deletion property
 *            with memoisation
 * Range:     u8 n <= 26, u16 n <= 542, u32 n <= 1511, u64 n <= 1511,
 *            u128 n <= 1511, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- Includes ---------- */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* Utility header for prime testing */
#include "../utils/is_prime.h"

/* ---------- Types from the build ---------- */
/* No definitions of seq_t or idx_t – supplied by the driver code. */

/* ---------- Global constants and storage ---------- */
/* Upper bound for memoisation: large enough for the first 10 000 BDP primes. */
static const idx_t BDP_CACHE_LIMIT = 10000000ULL;

/* Memoisation table for base‑5 deletable primes: 0 = unknown, 1 = false, 2 = true. */
static uint8_t *bdp_cache = NULL;       /* size = BDP_CACHE_LIMIT + 1 */
static bool cache_inited = false;

/* Cached list of BDP primes that have been found so far. */
static seq_t *bdp_list = NULL;          /* dynamic array of seq_t */
static size_t bdp_len   = 0;            /* current size */
static size_t bdp_cap   = 0;            /* allocated capacity */

/* Last integer that was tested for BDP status. */
static idx_t test_from = 2;             /* numbers < test_from already processed */

/* ---------- Helper: initialise memoisation table ---------- */
static void init_cache(void)
{
    if (!cache_inited) {
        bdp_cache = (uint8_t*)calloc((size_t)BDP_CACHE_LIMIT + 1, sizeof(uint8_t));
        cache_inited = true;
    }
}

/* ---------- Helper: resize the cached prime list ---------- */
static void grow_bdp_list(size_t needed)
{
    if (bdp_cap >= needed) return;
    size_t new_cap = bdp_cap ? bdp_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_ptr = (seq_t*)realloc(bdp_list, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();   /* no memory – give up */
    bdp_list = new_ptr;
    bdp_cap = new_cap;
}

/* ---------- Recursive test for base‑5 deletable prime, with memoisation ---------- */
static bool compute_bdp(idx_t p)
{
    /* Base case: a single decimal digit 2 or 3 is automatically deletable. */
    if (p < 5) return is_prime(p);   /* primes <5 are 2,3 */

    /* Convert p to base‑5 digits (most significant first). */
    uint8_t digits[25];   /* enough for p < 5e10, far beyond our bound */
    int len = 0;
    idx_t tmp = p;
    do {
        digits[len++] = (uint8_t)(tmp % 5);
        tmp /= 5;
    } while (tmp > 0);

    /* Reverse to get most significant digit first. */
    for (int i = 0, j = len - 1; i < j; ++i, --j) {
        uint8_t d = digits[i];
        digits[i] = digits[j];
        digits[j] = d;
    }

    /* Try removing each digit. Skip if the removal would leave a leading zero. */
    for (int pos = 0; pos < len; ++pos) {
        if (pos == 0 && len > 1 && digits[1] == 0) continue;   /* leading zero after deletion */

        /* Compute the number obtained by skipping digit pos. */
        idx_t q = 0;
        for (int k = 0; k < len; ++k) {
            if (k == pos) continue;
            q = q * 5 + digits[k];
        }

        /* q must be prime and deletable. */
        if (!is_prime(q)) continue;

        bool ok;
        if (q < BDP_CACHE_LIMIT) {
            uint8_t cached = bdp_cache[(size_t)q];
            if (cached) ok = (cached == 2);
            else {
                ok = compute_bdp(q);
                bdp_cache[(size_t)q] = ok ? 2 : 1;
            }
        } else {
            ok = compute_bdp(q);   /* recursion will not store beyond limit */
        }

        if (ok) return true;
    }
    return false;
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but guard against underflow */

    init_cache();

    /* Ensure the cached list is long enough. */
    while (bdp_len < (size_t)n) {
        /* Scan successive primes. */
        while (true) {
            if (!is_prime(test_from)) { test_from++; continue; }

            bool res;
            if (test_from < BDP_CACHE_LIMIT) {
                uint8_t cached = bdp_cache[(size_t)test_from];
                if (cached) res = (cached == 2);
                else {
                    res = compute_bdp(test_from);
                    bdp_cache[(size_t)test_from] = res ? 2 : 1;
                }
            } else {
                res = compute_bdp(test_from);   /* beyond cache */
            }

            if (res) {
                grow_bdp_list(bdp_len + 1);
                bdp_list[bdp_len++] = (seq_t)test_from;
            }
            test_from++;
            break;   /* one prime processed, check list size */
        }
    }

    return bdp_list[n - 1];
}

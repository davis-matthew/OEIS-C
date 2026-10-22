/*
 * A325261  Numbers whose omega-sequence does not cover an initial interval of
 *          positive integers.
 * https://oeis.org/A325261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   bigomega, factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A325261: Numbers whose ω-sequence does not cover an initial
   interval of positive integers.  The function a(n) returns the nth
   element of the sequence (offset 1).  The implementation searches
   integers incrementally and tests each candidate; the test uses the
   definition of the ω‐sequence as outlined in the OEIS entry.   */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/bigomega.h"
#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"

/* Helper:apply red(n) = Π prime(e_i) where n = Π p_i^{e_i}*/
static uint64_t red_uint64(uint64_t n)
{
    if (n <= 1) return 1;
    uint64_t pr[32], ex[32];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);
    __uint128_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint32_t p = get_prime((idx_t)ex[i]);    /* prime of the exponent */
        prod *= p;
        if (prod > (__uint128_t)UINT64_MAX)      /* safety */
            return UINT64_MAX;
    }
    return (uint64_t)prod;
}

/* Determine whether integer n belongs to the sequence. */
static bool qualifies(uint64_t n)
{
    if (n <= 1) return false;       /* avoid infinite loop for n = 0 or 1 */

    bool seen[256] = {0};
    uint64_t cur = n;
    uint64_t maxv = 0;

    for (;;) {
        seq_t ome = bigomega((idx_t)cur);   /* Ω(cur), 0 ≤ ome < 256 for 64‑bit n */
        uint64_t o = (uint64_t)ome;
        if (o >= sizeof(seen)) break;       /* too large – stop early */
        seen[o] = true;
        if (o > maxv) maxv = o;
        if (o == 1) break;
        cur = red_uint64(cur);
        if (cur == 1) break;               /* safety guard */
    }

    for (uint64_t i = 1; i <= maxv; ++i)
        if (!seen[(size_t)i]) return true; /* missing an integer */
    return false;                         /* covers initial interval */
}

/* ----------  Dynamic array storage for computed terms  ---------- */
static seq_t *seq_arr   = NULL;
static size_t seq_cap   = 0;
static size_t seq_len   = 0;
static uint64_t next_candidate = 2;         /* start searching from 2 */

/* Ensure enough capacity for the array */
static void ensure_capacity(size_t needed)
{
    if (seq_cap < needed) {
        size_t newcap = (seq_cap > 0) ? seq_cap * 2 : 16;
        if (newcap < needed) newcap = needed;
        seq_arr = (seq_t *)realloc(seq_arr, newcap * sizeof(seq_t));
        seq_cap = newcap;
    }
}

/* ----------  Public interface required by the build  ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* offset 1: undefined for n = 0 */
    while (seq_len < (size_t)n) {             /* extend the sequence as needed */
        uint64_t cand = next_candidate;
        while (!qualifies(cand)) ++cand;
        ensure_capacity(seq_len + 1);
        seq_arr[seq_len++] = (seq_t)cand;
        next_candidate = cand + 1;
    }
    return seq_arr[(size_t)n - 1];
}

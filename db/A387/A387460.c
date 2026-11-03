/*
 * A387460  Composite cyclic numbers (subset of A050384) that are of the form
 *          4n+1.
 * https://oeis.org/A387460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A387460 – composite cyclic numbers of the form 4k+1
 * Range:     u8 n <= 16, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   gcd, is_prime_u64, mem, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function returns the n-th element of the sequence.  It uses
    a simple incremental search starting from the smallest number,
    retaining a static cache so that calls with arbitrary n are fast.  */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/gcd.h"
#include "../utils/totient_seq.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, return 0 for safety */

    /* Static cache of computed terms */
    static seq_t *cache = NULL;           /* array of seq_t values          */
    static size_t cache_len = 0;          /* number of terms stored        */
    static size_t cache_cap = 0;          /* allocated capacity of array    */
    static idx_t last_candidate = 1;      /* last tested number            */

    /* Grow the cache array if necessary */
    if (cache_len < n) {
        size_t needed = n;
        if (cache_cap < needed) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) {
                /* allocation failed – in a real program you might abort */
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Search for the next terms until we have n of them */
        while (cache_len < n) {
            /* Advance to the next integer of the form 4k+1 */
            last_candidate++;
            if (last_candidate % 4 != 1)
                continue;

            /* Convert to seq_t for operations that need it */
            seq_t num = (seq_t)last_candidate;

            /* Skip primes – we only want composite numbers */
            if (is_prime_big(num))
                continue;

            /* Compute Euler's totient */
            seq_t phi_val = totient_seq(num);

            /* Check cyclicity: gcd(num, phi_val) == 1 */
            if (gcd(num, phi_val) == (seq_t)1) {
                cache[cache_len++] = num;
            }
        }
    }

    /* Return the requested term (1‑based index) */
    return cache[n - 1];
}

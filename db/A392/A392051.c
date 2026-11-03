/*
 * A392051  Nonnegative integers such that the harmonic mean of the digits is
 *          an integer.
 * https://oeis.org/A392051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A392051: a(n) = the nth nonnegative integer whose digits' harmonic
 * mean is an integer.  The function scans integers and checks the property
 * that (n * Πd) / Σ(Π d except one) is integral, treating a zero denominator
 * as giving the value 0. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/* helper: test whether k has integer harmonic mean of its decimal digits */
static bool harmonic_integer(seq_t k)
{
    /* store digits and compute product */
    uint8_t digits[32];   /* enough for any index up to 10⁹ */
    size_t len = 0;

    if (k == 0) {
        digits[len++] = 0;
    } else {
        seq_t tmp = k;
        while (tmp != 0) {
            digits[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }

    /* pref[i] = product of digits[0..i-1], pref[0] = 1 */
    seq_t pref[33];
    pref[0] = 1;
    for (size_t i = 0; i < len; ++i)
        pref[i + 1] = pref[i] * (seq_t)digits[i];

    /* suff[i] = product of digits[i..len-1], suff[len] = 1 */
    seq_t suff[33];
    suff[len] = 1;
    for (size_t i = len; i-- > 0; )
        suff[i] = suff[i + 1] * (seq_t)digits[i];

    seq_t P = pref[len];   /* product of all digits */
    seq_t S = 0;           /* sum of products of all digits except one */
    for (size_t i = 0; i < len; ++i)
        S += pref[i] * suff[i + 1];

    seq_t Hnum = (seq_t)len * P;   /* numerator of the harmonic mean */

    if (S != 0)
        return (Hnum % S == 0);
    return (Hnum == 0);   /* corresponds to 0/0 treated as 0 */
}

/* cache of discovered terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static seq_t next_candidate = 0;

/* a(n) – nth term of the sequence (offset 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* safety: offset is 1 */

    while (cache_size < (size_t)n) {
        if (harmonic_integer(next_candidate)) {
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();   /* out‑of‑memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}

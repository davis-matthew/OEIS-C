/*
 * A352179  Numbers k such that the decimal expansion of 14^k begins with 14.
 * https://oeis.org/A352179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1966, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that the decimal expansion of 14^k begins with 14
   (OEIS A352179).  The method tests each integer k once, using rational
   arithmetic with a 10¹⁵‑scaled approximation of log₁₀(14).  The fractions
   are compared directly, so no floating point is used. */

#include <stdlib.h>
#include <stdint.h>

/* 10¹⁵ scaling factor for log₁₀(14).  The constants are rounded to the
   nearest integer.  Using a larger denominator keeps the approximation
   accurate for the required range. */
#define LOG14_DEN 1000000000000000ULL      /* 10¹⁵                   */
#define LOG14_NUM 1146128035417942ULL      /* log₁₀(14) * 10¹⁵       */
#define LB_NUM    146128035417942ULL       /* log₁₀(1.4) * 10¹⁵    */
#define UB_NUM    176091259055681ULL       /* log₁₀(1.5) * 10¹⁵    */

/* Test whether a given k gives a leading decimal pair 14 in 14^k.
   The function computes (k∙log₁₀(14)) modulo 10¹⁵, then checks that
   the fractional part lies in [log₁₀(1.4), log₁₀(1.5)).  All arithmetic
   stays in uint64_t except for the intermediate 128‑bit product. */
static int qualifies(idx_t k)
{
    /* Multiply k by the 10¹⁵‑scaled log.  The product fits in 128 bits
       for all k up to at least 10⁶. */
    unsigned __int128 prod = (unsigned __int128)k * LOG14_NUM;
    /* remainder of division by the scaling factor gives the scaled
       fractional part. */
    uint64_t rem = (uint64_t)(prod % LOG14_DEN);

    /* The fractional part is rem / LOG14_DEN.  It must be ≥ LB_NUM / LOG14_DEN
       and < UB_NUM / LOG14_DEN.  Because the denominator is the same for
       all three numbers we can compare the numerators directly. */
    if (rem < LB_NUM || rem >= UB_NUM)
        return 0;
    return 1;
}

/* Return the nth term (n ≥ 1) of the sequence.  The function keeps a
   static cache of all k values found so far; it tests each successive
   integer once, so the cost is linear in the value of the result. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for offset 1, but guard anyway */

    static seq_t *cache = NULL;      /* array of found k's (seq_t)       */
    static size_t cache_len = 0;     /* number of cached terms           */
    static size_t cache_cap = 0;     /* allocated capacity               */
    static idx_t next_k = 1;         /* next integer to test            */

    /* Grow cache until it contains at least n terms */
    while (cache_len < n) {
        /* Find next qualifying k */
        while (!qualifies(next_k))
            ++next_k;

        /* Grow array if needed */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Append to cache */
        cache[cache_len++] = (seq_t)next_k;
        ++next_k;   /* prepare for next search */
    }

    return cache[n - 1];
}

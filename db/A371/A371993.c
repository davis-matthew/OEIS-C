/*
 * A371993  The smallest number k that has a factorization k = f1*f2*...*fr
 *          where the digits of {k, f1, f2, ..., fr} together contain all, and
 *          only, the digits 0..n, where n<=9.
 * https://oeis.org/A371993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, changed
 * Formula:   The smallest number k that has a factorization k = f1*f2*...*fr
 *            where the digits of {k, f1, f2, ..., fr} together contain all,
 *            and only, the digits 0..n (n ≤ 9). The sequence is indexed from
 *            1. The function a(n) is computed by brute‑force search and
 *            cached for all 1 ≤ n ≤ 9
 * Range:     u8 none, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: full, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the bit mask of decimal digits present in x.
   Bits 0..9 denote digits 0..9. */
static unsigned digit_mask_calc(seq_t x)
{
    unsigned m = 0;
    if (x == 0)   /* the number 0 itself */
        return 1u;            /* only bit 0 set */

    while (x > 0) {
        unsigned d = (unsigned)(x % 10);
        m |= 1u << d;
        x /= 10;
    }
    return m;
}

/* Depth‑first search for a valid factorization of k.
   Parameters:
     rem          – part of k still to factor
     last         – lower bound for next factor to keep order
     mask         – current union of digit masks (includes digits of k
                    and already chosen factors)
     depth        – number of factors chosen so far
     requiredMask – bit mask of required digits {0,…,n} */
static bool search_factorization(seq_t k, seq_t rem, seq_t last,
                                 unsigned mask, unsigned depth,
                                 unsigned requiredMask)
{
    if (rem == 1) {                     /* all factors chosen */
        return (depth >= 2) && (mask == requiredMask);
    }

    /* Try factors f within [last, sqrt(rem)] */
    for (seq_t f = last; f * f <= rem; ++f) {
        if (rem % f != 0) continue;

        unsigned mf = digit_mask_calc(f);
        /* reject if f contains a forbidden digit or introduces wrong digits */
        if ((mf & ~requiredMask) != 0) continue;
        if ((mask | mf) & ~requiredMask) continue;

        if (search_factorization(k, rem / f, f, mask | mf,
                                 depth + 1, requiredMask))
            return true;
    }

    /* Consider the remaining factor rem itself (if ≥ last) */
    if (rem >= last) {
        unsigned mf = digit_mask_calc(rem);
        if (((mf & ~requiredMask) == 0) &&
            (((mask | mf) & ~requiredMask) == 0)) {
            if (search_factorization(k, 1, rem, mask | mf,
                                     depth + 1, requiredMask))
                return true;
        }
    }
    return false;
}

/* Compute a(n) for 1 ≤ n ≤ 9 by brute force. */
static seq_t compute_a_for_n(unsigned n)
{
    unsigned requiredMask = (1u << (n + 1)) - 1u;   /* digits 0..n */
    seq_t k = 1;
    while (1) {
        /* Quick filter: k must not contain a digit > n */
        unsigned mk = 0;
        seq_t t = k;
        bool bad = false;
        if (t == 0) mk = 1u;          /* digit 0 */
        while (t > 0) {
            unsigned d = (unsigned)(t % 10);
            if (d > n) { bad = true; break; }
            mk |= 1u << d;
            t /= 10;
        }
        if (bad) { ++k; continue; }

        if (search_factorization(k, k, 2, mk, 0, requiredMask))
            return k;
        ++k;
    }
}

/* Public sequence function a(n) with external linkage. */
seq_t a(idx_t n)
{
    /* Cache values for 1 ≤ n ≤ 9.  The sequence is undefined for n>9. */
    static seq_t cache[10];
    static bool ready = false;
    if (!ready) {
        for (idx_t i = 1; i <= 9; ++i)
            cache[i] = compute_a_for_n((unsigned)i);
        ready = true;
    }
    if (n >= 1 && n <= 9)
        return cache[(idx_t)n];
    return 0;   /* undefined for n > 9 */
}

/*
 * A343552  Numbers m such that for some number k dividing n, m is formed by
 *          inserting a digit 0 between each pair of digits of k.
 * https://oeis.org/A343552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A343552: numbers m such that m is obtained by inserting a digit 0 between every pair of
   digits of some integer k and k divides m.
   The function generates the terms by scanning k ≥ 10 in increasing order and selecting
   those where f(k) mod k == 0.  The resulting m are stored in increasing order. */

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

/* Sequence generator */
seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *seq   = NULL;   /* array of terms                         */
    static size_t seq_len = 0;    /* number of stored terms                 */
    static size_t seq_cap = 0;    /* allocated capacity                     */
    static idx_t  next_k   = 10;  /* next k to be examined (≥ 10)           */

    while (seq_len < n)          /* generate terms until we have n of them */
    {
        uint64_t k = (uint64_t)next_k;          /* current k                         */
        uint64_t t = k;                         /* copy for digit extraction        */
        unsigned char digits[32];               /* digits of k, least‑significant first */
        size_t len = 0;
        while (t) {                            /* collect digits                    */
            digits[len++] = (unsigned char)(t % 10);
            t /= 10;
        }
        if (len == 0) {                        /* never happens for k ≥ 10           */
            digits[len++] = 0;
        }

        /* Remainder of m = f(k) modulo k */
        uint64_t rem = 0;
        __int128 m128 = 0;                     /* to compute m without overflow    */
        for (int i = (int)len - 1; i >= 0; --i) {
            unsigned d = digits[i];
            rem = (uint64_t)(((unsigned __int128)rem * 100 + d) % k);
            m128 = m128 * 100 + d;
        }

        if (rem == 0) {                        /* k divides f(k) – keep the term    */
            seq_t m_value;

            /* Handle small integer builds (e.g. 64‑bit) */
            if (sizeof(seq_t) <= sizeof(unsigned long long)) {
                if (m128 <= (__int128)ULLONG_MAX) {
                    m_value = (seq_t)m128;
                } else {                     /* further terms will overflow        */
                    break;                   /* the sequence only gets larger      */
                }
            } else {                           /* arbitrary‑precision builds          */
                seq_t temp_m = 0;
                for (int i = (int)len - 1; i >= 0; --i)
                    temp_m = temp_m * 100 + digits[i];
                m_value = temp_m;
            }

            /* Ensure capacity for the cache */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = m_value;          /* store the term */
        }

        ++next_k;                              /* advance to next candidate k */
    }

    return seq[n - 1];
}

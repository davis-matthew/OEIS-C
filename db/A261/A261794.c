/*
 * A261794  a(n) is the smallest nonzero number that is not a substring of n
 *          in decimal representation.
 * https://oeis.org/A261794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A261794: the smallest positive integer that does not occur as a
   decimal substring of the argument n. */
#include <stdio.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert n to its decimal string representation */
    char n_str[64];
    (void)snprintf(n_str, sizeof n_str, "%llu", (unsigned long long)n);

    /* Search for the smallest m >= 1 not appearing in n_str */
    for (idx_t m = 1; ; ++m) {
        char m_str[64];
        (void)snprintf(m_str, sizeof m_str, "%llu", (unsigned long long)m);
        if (!strstr(n_str, m_str))
            return (seq_t)m;   /* cast works for both built‑in and big‑int seq_t */
    }
}

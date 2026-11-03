/*
 * A262289  a(n) = smallest positive number, not a power of 2, that is not a
 *          substring of n in its binary representation.
 * https://oeis.org/A262289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest positive number, not a power of 2, that is not a
 *            substring of n in its binary representation
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9421 (2026-11-01)
 * Helpers:   is_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_pow2.h"

/* Convert a non‑negative idx_t value to a binary string.
   The result is stored in buf which must be large enough
   (at least 65 characters for a 64‑bit value).  The length of
   the binary string (not counting the terminating NUL) is
   stored in *len.  */
static void to_binary_string(idx_t v, char *buf, size_t *len)
{
    if (v == 0) {
        *len = 1;
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    size_t l = 0;
    idx_t tmp = v;
    while (tmp != 0) {
        tmp >>= 1;
        ++l;
    }

    *len = l;
    buf[l] = '\0';
    /* Fill from the end towards the beginning. */
    for (size_t i = 0; i < l; ++i) {
        buf[l - 1 - i] = (v & 1) ? '1' : '0';
        v >>= 1;
    }
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* Binary representation of the index n. */
    char n_str[65];
    size_t n_len;
    to_binary_string(n, n_str, &n_len);

    seq_t m = 1;
    char m_str[65];
    size_t m_len;

    for (;;) {
        if (!is_pow2(m)) {
            /* Convert m to binary for comparison. */
            idx_t mv = (idx_t)m;      /* values here are tiny */
            to_binary_string(mv, m_str, &m_len);

            /* If the candidate is longer than the index string, it
               cannot appear as a substring. */
            if (m_len > n_len)
                return m;

            /* Simple substring search – lengths are small. */
            bool found = false;
            for (size_t i = 0; i + m_len <= n_len; ++i) {
                bool ok = true;
                for (size_t j = 0; j < m_len; ++j) {
                    if (n_str[i + j] != m_str[j]) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    found = true;
                    break;
                }
            }
            if (!found)
                return m;
        }
        ++m;  /* Next candidate. */
    }
}

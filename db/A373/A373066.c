/*
 * A373066  Numbers k such that the Hankel matrix formed using the digits of k
 *          is singular.
 * https://oeis.org/A373066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 438, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9735 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A373066 – Numbers k such that the Hankel matrix
   formed from the decimal digits of k is singular.
   Computation is performed by enumerating integers in order and
   testing singularity via the Bareiss algorithm on the Hankel matrix. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Test whether the Hankel matrix built from the decimal digits
   of `num` has determinant 0. */
static bool hankel_singular(idx_t num)
{
    /* Extract digits in reverse order */
    uint8_t rev[64];
    size_t pos = 0;
    idx_t u = num;
    if (u == 0) {
        rev[pos++] = 0;
    } else {
        while (u > 0) {
            rev[pos++] = (uint8_t)(u % 10);
            u /= 10;
        }
    }

    /* Odd number of digits is required */
    if ((pos & 1) == 0)
        return false;

    /* Reverse digits into normal order */
    uint8_t digits[64];
    for (size_t i = 0; i < pos; ++i)
        digits[i] = rev[pos - 1 - i];

    size_t len = pos;
    size_t s   = (len + 1) >> 1;          /* matrix dimension */

    /* The maximum number of digits we expect for the target range
       (n ≤ 10000) is well below 64, so a statically sized matrix is
       sufficient. */
    int64_t m[32][32] = {0};
    for (size_t i = 0; i < s; ++i)
        for (size_t j = 0; j < s; ++j)
            m[i][j] = digits[i + j];

    int64_t prev = 1;
    for (size_t k = 0; k < s; ++k) {
        /* Find a non‑zero pivot by swapping rows if necessary */
        if (m[k][k] == 0) {
            size_t r;
            for (r = k + 1; r < s; ++r)
                if (m[r][k] != 0)
                    break;
            if (r == s)   /* all entries in column k below or equal to k are 0 */
                return true;          /* determinant is 0 */
            /* swap rows k and r */
            for (size_t c = 0; c < s; ++c) {
                int64_t tmp = m[k][c];
                m[k][c] = m[r][c];
                m[r][c] = tmp;
            }
        }

        int64_t pivot = m[k][k];
        for (size_t i = k + 1; i < s; ++i)
            for (size_t j = k + 1; j < s; ++j)
                /* Bareiss update */
                m[i][j] = (m[i][j] * pivot - m[i][k] * m[k][j]) / prev;
        prev = pivot;
    }

    return m[s - 1][s - 1] == 0;
}

/* Sequence cache – a static array of maximum size needed for the tests
   (enough for n up to about 10000). */
enum { MAX_CAP = 200000 };
static seq_t values[MAX_CAP];
static size_t val_len = 0;
static idx_t next_candidate = 0;

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Guard against too large n – the array has been chosen large
       enough for the expected limits. */
    if (n == 0)
        return 0;        /* should never happen for the OEIS offset */

    while (val_len < (size_t)n) {
        if (hankel_singular(next_candidate)) {
            if (val_len < MAX_CAP)
                values[val_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return values[(size_t)n - 1];
}

/*
 * A259228  Lexicographically earliest sequence of distinct terms such that
 *          the first significant digits of 1/n start with a(n).
 * https://oeis.org/A259228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For each n, find the lexicographically earliest unused integer
   that appears as a prefix of the decimal expansion of 1/n (ignoring
   leading zeros after the decimal point).  The function stores the
   sequence up to the requested index in static arrays. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Offsets and types are supplied by the build system.
       idx_t : unsigned 64‑bit index (e.g. uint64_t)
       seq_t : unsigned integer of 8,16,32,64,128 bits or arbitrary big int   */

    /* static storage for the sequence and the set of used numbers */
    static seq_t *seq          = NULL;
    static size_t seq_len      = 0, seq_cap      = 0;
    static seq_t *used         = NULL;
    static size_t used_len     = 0, used_cap     = 0;

    if (n == 0) /* safety; OEIS offset is 1 */
        return (seq_t)0;

    /* Build sequence up to requested index n */
    while (seq_len < (size_t)n) {
        seq_t val;
        idx_t idx = (idx_t)(seq_len + 1);   /* 1‑based index */

        /* First term is always 1 */
        if (seq_len == 0) {
            val = (seq_t)1;
        } else {
            idx_t denom = idx;
            idx_t rem   = 1;          /* start with numerator 1 */
            bool started = false;
            seq_t candidate = 0;
            bool found = false;

            /* Generate digits of 1/denom in decimal */
            while (!found) {
                rem *= (idx_t)10;
                idx_t digit_idx = rem / denom;
                rem %= denom;
                seq_t digit = (seq_t)digit_idx;

                /* Skip leading zeros after decimal point */
                if (!started) {
                    if (digit == (seq_t)0)
                        continue;
                    started = true;
                }

                /* Append digit to candidate */
                candidate = candidate * (seq_t)10 + digit;

                /* Check if candidate has been used already */
                bool used_flag = false;
                for (size_t i = 0; i < used_len; ++i) {
                    if (used[i] == candidate) {
                        used_flag = true;
                        break;
                    }
                }

                if (!used_flag) {
                    val = candidate;
                    found = true;
                }
            }
        }

        /* Append val to used set */
        if (used_len == used_cap) {
            size_t new_cap = used_cap ? used_cap * 2 : 4;
            seq_t *tmp = (seq_t *)realloc(used, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            used = tmp;
            used_cap = new_cap;
        }
        used[used_len++] = val;

        /* Append val to sequence array */
        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 4;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            seq = tmp;
            seq_cap = new_cap;
        }
        seq[seq_len++] = val;
    }

    return seq[n - 1];
}

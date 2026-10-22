/*
 * A393101  Number of character comparisons needed by Knuth-Morris-Pratt
 *          string search to identify the first occurrence of string n in the
 *          infinite Champernowne word 123456789101112... (A033307).
 * https://oeis.org/A393101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 117, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of KMP comparison count for the Champernowne word */
/* The function returns the number of character comparisons needed
   by the Knuth-Morris-Pratt search to find the first occurrence of
   the decimal representation of n in 123456789101112...            */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert n to its decimal string representation (the pattern). */
    char pat[32];
    int len = snprintf(pat, sizeof pat, "%llu", (unsigned long long)n);
    /* Build the KMP failure function. */
    int pi[32] = {0};
    for (int i = 1; i < len; ++i) {
        int j = pi[i - 1];
        while (j > 0 && pat[i] != pat[j])
            j = pi[j - 1];
        if (pat[i] == pat[j])
            ++j;
        pi[i] = j;
    }

    seq_t comparisons = 0;
    idx_t num = 1;          /* Current integer being written to the word. */
    int j = 0;              /* Current length of matched prefix. */

    while (1) {
        /* Convert the current number to a string. */
        char buf[32];
        int slen = snprintf(buf, sizeof buf, "%llu",
                            (unsigned long long)num);

        for (int pos = 0; pos < slen; ++pos) {
            char c = buf[pos];
            int j_local = j;

            for (;;) {
                comparisons++;           /* Count this comparison. */
                if (c == pat[j_local]) { /* Match */
                    ++j_local;
                    if (j_local == len) /* Pattern found */
                        return comparisons;
                    break;
                }
                /* Mismatch */
                if (j_local == 0)       /* No fallback possible */
                    break;

                int j_back = pi[j_local - 1];
                /* If the next candidate pattern character equals the
                   current one, the second comparison would fail as well
                   (c != pat[j_local]), so skip counting it. */
                if (pat[j_back] == pat[j_local]) {
                    j_local = j_back;
                    break;           /* Skip the redundant comparison */
                }
                /* Need to compare again with the new prefix. */
                j_local = j_back;
                /* Continue loop; the next iteration will count the
                   second comparison. */
            }
            j = j_local;               /* Restore state for next text char */
        }
        ++num;
    }
}

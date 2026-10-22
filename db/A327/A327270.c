/*
 * A327270  The minimum number such that the concatenation of the absolute
 *          values of differences between adjacent digits of a(n) is n. Values
 *          of n which have no such a(n) are given as -1.
 * https://oeis.org/A327270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = minimum integer x such that the concatenation of |d_i -
 *            d_{i+1}| over all adjacent decimal digits d_i of x equals n;
 *            returns -1 (converted to seq_t) if no such x exists
 * Range:     i8 n <= 19, i16 n <= 1047, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)-1;
    }

    /* Convert n to its decimal digit array (most significant digit first). */
    unsigned char n_digits[25];   /* uint64_t holds at most 20 decimal digits. */
    size_t nlen = 0;
    uint64_t tmp = n;
    while (tmp > 0) {
        n_digits[nlen++] = tmp % 10;
        tmp /= 10;
    }
    /* Reverse to obtain MSD first. */
    for (size_t i = 0; i < nlen / 2; ++i) {
        unsigned char t = n_digits[i];
        n_digits[i] = n_digits[nlen - 1 - i];
        n_digits[nlen - 1 - i] = t;
    }

    /* DP node structure. */
    struct Node {
        bool exist;
        unsigned char digits[25];
        size_t len;
    };
    struct Node nodes[21][10];   /* 21 positions: 0..nlen (max 20) and 10 digits. */

    /* Base case: at position nlen, suffix consists of the single current digit. */
    for (int d = 0; d < 10; ++d) {
        nodes[nlen][d].exist = true;
        nodes[nlen][d].len   = 1;
        nodes[nlen][d].digits[0] = (unsigned char)d;
    }

    /* Dynamic programming from nlen-1 down to 0. */
    for (int pos = (int)nlen - 1; pos >= 0; --pos) {
        unsigned char diff = n_digits[pos];
        for (int d = 0; d < 10; ++d) {
            bool any = false;
            unsigned char best[25];
            size_t best_len = 0;

            /* Candidate 1: next digit = d + diff */
            int nd = d + diff;
            if (nd <= 9 && nodes[pos + 1][nd].exist) {
                best_len = 1 + nodes[pos + 1][nd].len;
                best[0] = (unsigned char)d;
                memcpy(&best[1], nodes[pos + 1][nd].digits, nodes[pos + 1][nd].len);
                any = true;
            }

            /* Candidate 2: next digit = d - diff */
            nd = d - diff;
            if (nd >= 0 && nodes[pos + 1][nd].exist) {
                unsigned char cand[25];
                size_t cand_len = 1 + nodes[pos + 1][nd].len;
                cand[0] = (unsigned char)d;
                memcpy(&cand[1], nodes[pos + 1][nd].digits, nodes[pos + 1][nd].len);

                if (!any || memcmp(cand, best, cand_len) < 0) {
                    any = true;
                    best_len = cand_len;
                    memcpy(best, cand, cand_len);
                }
            }

            if (any) {
                nodes[pos][d].exist = true;
                nodes[pos][d].len   = best_len;
                memcpy(nodes[pos][d].digits, best, best_len);
            } else {
                nodes[pos][d].exist = false;
            }
        }
    }

    /* Choose the minimal number with a leading digit ≠ 0. */
    bool found = false;
    unsigned char result_digits[25];
    size_t result_len = 0;
    for (int d = 1; d <= 9; ++d) {
        if (nodes[0][d].exist) {
            if (!found || memcmp(nodes[0][d].digits, result_digits, nodes[0][d].len) < 0) {
                found = true;
                result_len = nodes[0][d].len;
                memcpy(result_digits, nodes[0][d].digits, result_len);
            }
        }
    }

    if (!found) {
        return (seq_t)-1;
    }

    /* Convert digit array back to seq_t number. */
    seq_t res = 0;
    for (size_t i = 0; i < result_len; ++i) {
        res = res * 10 + result_digits[i];
    }
    return res;
}

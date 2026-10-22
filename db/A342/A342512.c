/*
 * A342512  a(n) is the number of substrings of the binary representation of n
 *          that are instances of the Zimin word Z_k, where k = A342510(n).
 * https://oeis.org/A342512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A342511(n, A342510(n)) For a fixed k (maximum Zimin word
 *            appearing in the binary expansion of n), a(n) counts all
 *            substrings that are instances of the Zimin word Z_k. The
 *            algorithm is: 1. Convert n to its binary string. 2. Determine
 *            the largest k such that the string contains an instance o...
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   build_pat_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_pat_rec.h"

/* Maximum k needed for 64‑bit numbers: 2^k - 1 <= 64  ->  k <= 6 */
#define MAX_K 6

/* Pre‑computed pattern and counts for each k */
static int pat_of_k[MAX_K + 1][64];     /* pattern variable indices */
static int patlen[MAX_K + 1];           /* pattern length (2^k - 1) */
static int occcnt[MAX_K + 1][MAX_K];    /* number of occurrences of each var */

/* Recursive assignment of lengths to variables */
static bool assign_rec(int var, int remaining, int *lenVar,
                       const int *cnt, int k,
                       const char *s, int slen,
                       const int *pat, int patlen)
{
    if (var == k - 1) {
        if (remaining % cnt[var] != 0) return false;
        int l = remaining / cnt[var];
        if (l < 1) return false;
        lenVar[var] = l;
        /* Check the pattern */
        int firstPos[MAX_K];
        for (int i = 0; i < k; ++i) firstPos[i] = -1;
        int pos = 0;
        for (int idx = 0; idx < patlen; ++idx) {
            int v = pat[idx];
            int L = lenVar[v];
            if (firstPos[v] == -1) {
                firstPos[v] = pos;
            } else {
                int fp = firstPos[v];
                for (int t = 0; t < L; ++t) {
                    if (s[pos + t] != s[fp + t]) return false;
                }
            }
            pos += L;
        }
        return pos == slen;
    }
    int maxL = remaining / cnt[var];
    for (int l = 1; l <= maxL; ++l) {
        lenVar[var] = l;
        int remAfter = remaining - cnt[var] * l;
        /* Minimum required for remaining variables */
        int minNeeded = 0;
        for (int i = var + 1; i < k; ++i) minNeeded += cnt[i];
        if (remAfter < minNeeded) continue;
        if (assign_rec(var + 1, remAfter, lenVar, cnt, k,
                       s, slen, pat, patlen))
            return true;
    }
    return false;
}

/* Public interface ----------------------------------------------------- */
seq_t a(idx_t n)
{
    /* ---------- prepare binary string of n --------------------------- */
    char bin[65];
    int len;
    if (n == 0) {
        bin[0] = '0';
        len = 1;
    } else {
        idx_t tmp = n;
        int pos = 0;
        while (tmp > 0) {
            bin[pos++] = (tmp & 1ULL) ? '1' : '0';
            tmp >>= 1ULL;
        }
        /* reverse to most‑significant bit first */
        for (int i = 0; i < pos / 2; ++i) {
            char t = bin[i];
            bin[i] = bin[pos - 1 - i];
            bin[pos - 1 - i] = t;
        }
        len = pos;
    }

    /* ---------- initialize pattern data on first call --------------- */
    static bool init = false;
    if (!init) {
        for (int k = 1; k <= MAX_K; ++k) {
            int pos = 0;
            build_pat_rec(k, pat_of_k[k], &pos);
            patlen[k] = pos;
            for (int i = 0; i < k; ++i) {
                occcnt[k][i] = 1 << (k - i - 1);
            }
        }
        init = true;
    }

    /* ---------- find largest k that has an instance -------------- */
    int sel_k = 1;          /* at least 1 always matches */
    int found = 0;
    for (int k = MAX_K; k >= 1; --k) {
        /* minimal length of Z_k is 2^k - 1 */
        int minLen = (1 << k) - 1;
        if (minLen > len) continue;   /* impossible for this k */

        int count = 0;
        for (int i = 0; i < len; ++i) {
            for (int j = i; j < len; ++j) {
                int sublen = j - i + 1;
                if (sublen < minLen) continue;
                if (k == 1) {
                    ++count;   /* any non‑empty substring matches Z_1 */
                } else {
                    int lenVar[MAX_K];
                    if (assign_rec(0, sublen, lenVar,
                                   occcnt[k], k,
                                   bin + i, sublen,
                                   pat_of_k[k], patlen[k]))
                        ++count;
                }
            }
        }
        if (count > 0) {
            found = 1;
            sel_k = k;
            /* we must still compute the correct count for this k,
               so we keep the computed count */
            return (seq_t)count;
        }
    }

    /* The loop above should always return, but for safety: */
    return 0;
}

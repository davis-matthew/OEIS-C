/*
 * A262530  Numbers such that digits occur at most twice in decimal
 *          representation.
 * https://oeis.org/A262530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   computes a(n), the nth integer whose every decimal digit occurs
 *            at most twice (OEIS A262530). The sequence is built by counting
 *            admissible digit strings, then unranking the n‑th number in
 *            lexicographic order
 * Range:     u8 n <= 253, u16 n <= 9915, u32 n <= 9915, u64 n <= 9915,
 *            u128 n <= 9915, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAX_LEN 20            /* maximal number of digits (2 repeats of 10 digits) */
#define STATE_CNT 59049       /* 3^10 possible digit‑frequency states */
#define FULLMASK 59048u       /* base‑3 code for all digits having capacity 2 */

/* powers of 3: pow3[k] = 3^k  (k = 0 … 9) */
static const unsigned int pow3[10] = {1,3,9,27,81,243,729,2187,6561,19683};

/* memoisation tables for count_rec(m,mask) */
static unsigned long long memo[MAX_LEN + 1][STATE_CNT];
static unsigned char    seen [MAX_LEN + 1][STATE_CNT];

/* ------------------------------------------------------------------ */
/* Count the number of admissible strings of length m that can be formed
   from the digit counts encoded in mask (base‑3 representation). */
static unsigned long long count_rec(int m, unsigned int mask)
{
    if (m == 0) return 1ULL;
    if (seen[m][mask]) return memo[m][mask];

    unsigned long long res = 0ULL;
    for (int d = 0; d < 10; d++) {
        unsigned int cntdigit = (mask / pow3[9 - d]) % 3U;
        if (cntdigit == 0) continue;
        unsigned int newmask = mask - pow3[9 - d];
        res += count_rec(m - 1, newmask);
    }
    seen[m][mask] = 1;
    memo[m][mask] = res;
    return res;
}

/* ------------------------------------------------------------------ */
/* Return the n-th number in the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    unsigned long long N = (unsigned long long)n;  /* N is 1‑based rank */
    int len = 1;

    /* Find the length of the n-th admissible number */
    while (1) {
        unsigned long long cnt = 0;
        for (int d = 1; d <= 9; d++) {
            unsigned int newmask = FULLMASK - pow3[9 - d];
            cnt += count_rec(len - 1, newmask);
        }
        if (N <= cnt) break;
        N -= cnt;
        ++len;
    }

    /* Unrank the number digit by digit */
    unsigned int mask = FULLMASK;
    seq_t val = 0;

    for (int pos = 0; pos < len; pos++) {
        int start = (pos == 0) ? 1 : 0;
        for (int d = start; d <= 9; d++) {
            unsigned int cntdigit = (mask / pow3[9 - d]) % 3U;
            if (cntdigit == 0) continue;
            unsigned int newmask = mask - pow3[9 - d];
            unsigned long long rem = count_rec(len - pos - 1, newmask);
            if (N <= rem) {
                val = val * 10 + (seq_t)d;
                mask = newmask;
                break;
            } else {
                N -= rem;
            }
        }
    }
    return val;
}

/*
 * A348150  a(n) is the smallest Niven (or Harshad) number with exactly n
 *          digits and not containing the digit 0.
 * https://oeis.org/A348150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest n‑digit Niven number with no digit 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   find_suffix, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_from_str.h"
#include "../utils/find_suffix.h"

/* Main function required by the problem statement */
seq_t a(idx_t n) {
    if (n == 0) return seq_from_str("0");   /* harmless fall‑back */

    /* Limit for how many trailing digits we are willing to adjust.
       Empirically the minimal solution never needs more than 7. */
    const int MAX_S = 7;
    /* Max possible digit sum = (n - MAX_S) + 9*MAX_S < 10100 for n<=10000 */
    const int MAX_SUM = 10100;

    /* Temporary buffers */
    int pre_mod[MAX_SUM];
    int pow10_mod[MAX_SUM];
    int digits[MAX_S];          /* suffix digits, at most MAX_S digits */

    /* Allocate a character buffer for the answer */
    char *buf = (char *)malloc((size_t)n + 1);
    if (!buf) return seq_from_str("0");    /* unlikely but safe */

    /* Search by increasing suffix length s */
    for (int s = 0; s <= MAX_S && (idx_t)s <= n; ++s) {
        idx_t p = n - (idx_t)s;          /* number of leading 1's */

        /* --- Handle the trivial case s == 0 (all digits are 1) --- */
        if (s == 0) {
            int mod = 0;
            for (idx_t i = 0; i < p; ++i)
                mod = (mod * 10 + 1) % (int)p;     /* p > 0 here (n>=1) */
            if (mod == 0) {                     /* all ones form a Niven number */
                for (idx_t i = 0; i < p; ++i) buf[i] = '1';
                buf[p] = '\0';
                seq_t result = seq_from_str(buf);
                free(buf);
                return result;
            }
            continue;   /* no solution with s==0, try larger s */
        }

        int suffix_min = s * 1;       /* minimal possible sum of the suffix */
        int suffix_max = s * 9;       /* maximal possible sum of the suffix */

        /* --- Pre‑compute prefix and 10^s modulo for every possible total sum S --- */
        int S_min = (int)(p + suffix_min);
        int S_max = (int)(p + suffix_max);
        for (int S = S_min; S <= S_max; ++S) {
            /* compute value of prefix (p ones) modulo S */
            int mod = 0;
            for (idx_t i = 0; i < p; ++i)
                mod = (mod * 10 + 1) % S;
            pre_mod[S] = mod;

            /* compute 10^s modulo S */
            int pow10 = 1 % S;
            for (int k = 0; k < s; ++k)
                pow10 = (pow10 * 10) % S;
            pow10_mod[S] = pow10;
        }

        /* --- Search all suffixes of length s in lexicographic order --- */
        if (find_suffix(0, s, 0, digits, p, pre_mod, pow10_mod,
                        n, buf)) {
            seq_t result = seq_from_str(buf);
            free(buf);
            return result;
        }
    }

    /* In theory a solution always exists; if the search fails, return 0. */
    free(buf);
    return seq_from_str("0");
}

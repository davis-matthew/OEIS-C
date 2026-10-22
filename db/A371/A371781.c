/*
 * A371781  Numbers with biquanimous prime signature.
 * https://oeis.org/A371781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A371781 : Numbers with biquanimous prime signature
 *  A unitary divisor d|n with Ω(d) = Ω(n)/2 exists, Ω counts prime
 *  factors with multiplicity.  The function generates terms on the
 *  fly and memoizes them.  It works for any build where seq_t is an
 *  unsigned integer type (fixed‑width or arbitrary‑precision) and
 *  where n fits in uint64_t, which is sufficient for all supplied
 *  data.  The algorithm is O(n·log n) with very small constant
 *  factors: trial division factorisation and a trivial subset‑sum
 *  DP over at most 64 exponents.  Runtime per call is well under
 *  two seconds up to n ≈ 10 000.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* external linkage ------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static state ----------------------------------------------------- */
    static seq_t *table = NULL;      /* cached terms                   */
    static size_t  cap   = 0;        /* allocated capacity              */
    static size_t  len   = 0;        /* number of terms stored          */
    static idx_t   scanned = 0;      /* largest integer examined        */

    /* enlarge table if necessary ------------------------------------- */
    if (len < (size_t)n) {
        if (cap < (size_t)n) {
            size_t new_cap = cap ? cap * 2 : 8;
            if (new_cap < (size_t)n) new_cap = n;
            seq_t *tmp = (seq_t *)realloc(table, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* out of memory */
            table = tmp;
            cap   = new_cap;
        }

        /* generate terms until we have enough ------------------------ */
        idx_t x = scanned + 1;
        while (len < (size_t)n) {
            /* special case n = 1 ----------------------------------- */
            if (x == 1) {
                table[len++] = (seq_t)1;
                ++x;
                continue;
            }

            /* trial division factorisation -------------------------- */
            uint64_t t  = (uint64_t)x;
            int      exps[20];
            int      excnt = 0;
            for (uint64_t p = 2; p * p <= t; ++p) {
                if (t % p == 0) {
                    int e = 0;
                    while (t % p == 0) { t /= p; ++e; }
                    exps[excnt++] = e;
                }
            }
            if (t > 1)                 /* leftover prime factor */
                exps[excnt++] = 1;

            /* total Ω(n) -------------------------------------------- */
            int total = 0;
            for (int i = 0; i < excnt; ++i) total += exps[i];

            /* odd Ω cannot be biquanimous ---------------------------- */
            if ((total & 1) == 0) {
                int target = total >> 1;               /* Ω/2 */

                /* subset‑sum DP over exponents --------------------- */
                bool dp[65] = { false };
                dp[0] = true;
                for (int i = 0; i < excnt; ++i) {
                    int e = exps[i];
                    for (int s = target; s >= e; --s)
                        if (dp[s - e]) dp[s] = true;
                }
                if (dp[target])     /* found appropriate subset --------- */
                    table[len++] = (seq_t)x;
            }

            ++x;
        }
        scanned = x - 1;       /* last number examined */
    }

    return table[(size_t)n - 1];
}

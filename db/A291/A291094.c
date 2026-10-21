/*
 * A291094  Denominators of fractions with nontrivial anomalous cancellation,
 *          listed with multiplicity if multiple numerators are possible.
 * https://oeis.org/A291094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, base
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 8544, u32 n <= 8544, u64 n <= 8544,
 *            u128 n <= 8544, big any n
 * Helpers:   digit_count64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the denominators of fractions with nontrivial anomalous
   cancellation.  The function enumerates denominators d starting from 11,
   finds all unreduced numerators n (10 ≤ n < d) whose fraction n/d has
   a single common digit that can be cancelled without changing the
   value, and counts the results in order.  The nth such denominator
   (offset 1) is returned.  This is an algorithmic implementation,
   not a closed formula. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_count64.h"

/* Small table of powers of ten (sufficient for 64‑bit values). */
static const unsigned long long POW10[20] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL, 1000000ULL,
    10000000ULL, 100000000ULL, 1000000000ULL, 10000000000ULL,
    100000000000ULL, 1000000000000ULL, 10000000000000ULL,
    100000000000000ULL, 1000000000000000ULL, 10000000000000000ULL,
    100000000000000000ULL, 1000000000000000000ULL
};

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */

    idx_t target   = n;            /* 1‑based target index */
    idx_t seen     = 0;            /* number of terms found so far */
    uint64_t d     = 11;           /* start with first allowed denominator */

    while (1) {
        /* Extract decimal digits of d into an array (least‑significant first). */
        uint64_t td = d;
        int dlen = 0;
        uint8_t digs[20];
        while (td > 0) {
            digs[dlen++] = (uint8_t)(td % 10);
            td /= 10;
        }

        /* Array to deduplicate numerators for this denominator. */
        uint64_t found_nums[32];
        size_t found_cnt = 0;

        /* For every digit position in the denominator. */
        for (int pos_d = 0; pos_d < dlen; ++pos_d) {
            int c     = digs[pos_d];          /* digit to cancel */
            uint64_t Td = POW10[pos_d];
            uint64_t A = d / (Td * 10ULL);
            uint64_t B = d % Td;

            /* M = Td*A + B, N = 10*Td*A + c*Td + B */
            uint64_t M = Td * A + B;
            uint64_t N = 10ULL * Td * A + (uint64_t)c * Td + B;

            /* Denominator of the linear equation |M - N| = Td*(9*A + c) */
            uint64_t denom_abs = Td * (9ULL * A + (uint64_t)c);
            if (denom_abs == 0) continue;     /* safety */

            /* Coefficient of C in the equation: 9*B - c*Td */
            int64_t coeff1 = (int64_t)9ULL * (int64_t)B - (int64_t)c * (int64_t)Td;

            /* For every position of the digit in the numerator. */
            for (int pos_n = 0; pos_n < dlen; ++pos_n) {
                uint64_t Tn = POW10[pos_n];
                if (Tn > d - 1) continue;      /* numerator must have this digit place */

                /* Maximum value for C so that the constructed numerator < d. */
                uint64_t bound = (d - Tn + 1ULL) / Tn;
                if (bound <= (uint64_t)c) continue;
                uint64_t maxC = (bound - (uint64_t)c) / 10ULL;

                for (uint64_t C = 0; C <= maxC; ++C) {
                    /* Compute numerator of linear equation for the removed part D. */
                    __int128 num = (__int128)Tn *
                                   (((__int128)coeff1 * C) + (__int128)M * c);

                    /* Must be divisible by denom_abs. */
                    if ((int64_t)(num % denom_abs) != 0) continue;

                    __int128 D = num / denom_abs;
                    if (D < 0 || D >= (__int128)Tn) continue;

                    /* Construct candidate numerator n. */
                    __int128 nval = (__int128)Tn * (((__int128)10 * C) + c) + D;
                    if (nval < 10) continue;           /* two‑digit minimum */
                    if (nval >= d) continue;          /* n < d required */

                    uint64_t nnum = (uint64_t)nval;

                    /* Exclude trivial fractions x0/y0. */
                    if ((nnum % 10ULL == 0ULL) && (d % 10ULL == 0ULL)) continue;

                    /* Deduplicate within this denominator. */
                    bool dup = false;
                    for (size_t k = 0; k < found_cnt; ++k)
                        if (found_nums[k] == nnum) { dup = true; break; }
                    if (!dup) {
                        if (found_cnt < sizeof(found_nums)/sizeof(found_nums[0]))
                            found_nums[found_cnt++] = nnum;
                    }
                }
            }
        }

        /* If this denominator yields any valid numerators. */
        if (found_cnt > 0) {
            if (seen + found_cnt >= target)
                return (seq_t)d;        /* the nth term is this denominator */

            seen += found_cnt;           /* accumulate count */
        }

        ++d;            /* proceed to next denominator */
    }
}

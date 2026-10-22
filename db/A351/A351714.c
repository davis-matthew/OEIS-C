/*
 * A351714  Lucas-Niven numbers: numbers that are divisible by the number of
 *          terms in their minimal (or greedy) representation in terms of the
 *          Lucas numbers (A130310).
 * https://oeis.org/A351714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Lucas‑Niven numbers. Numbers k such that the number of terms in
 *            their minimal representation via Lucas numbers divides k
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*--- Structure to store Lucas numbers --------------------------------- */
    static size_t luca_cap = 0, luca_len = 0;
    static seq_t *luca = NULL;

    /*--- Structure to store pre‑computed Lucas‑Niven numbers ------------ */
    static size_t niven_cap = 0, niven_len = 0;
    static seq_t *niven = NULL;
    static seq_t last_niven = 0;   /* last Lucas‑Niven value found */

    /*--- Initialise Lucas array once ------------------------------------- */
    if (luca_len == 0) {
        luca_cap = 16;
        luca = (seq_t *)malloc(luca_cap * sizeof(seq_t));
        luca[0] = (seq_t)2;   /* L0 = 2 */
        luca[1] = (seq_t)1;   /* L1 = 1 */
        luca_len = 2;
    }

    /*--- Initialise storage for Lucas‑Niven numbers ---------------------- */
    if (niven_cap == 0) {
        niven_cap = 16;
        niven = (seq_t *)malloc(niven_cap * sizeof(seq_t));
    }

    /*--- Generate terms until we have at least n of them ---------------- */
    while (niven_len < n) {
        /* Start testing from the next integer after the last found term */
        seq_t cand = last_niven + 1;

        /* Find the next Lucas‑Niven number by brute force */
        for (;;) {
            /* Grow Lucas array until it contains a value >= cand */
            while (luca_len > 0 && luca[luca_len - 1] < cand) {
                seq_t next = luca[luca_len - 1] + luca[luca_len - 2];
                if (luca_len == luca_cap) {
                    luca_cap *= 2;
                    luca = (seq_t *)realloc(luca, luca_cap * sizeof(seq_t));
                }
                luca[luca_len++] = next;
            }

            /* Compute the greedy Lucas representation of cand */
            seq_t rem = cand;
            unsigned int cnt = 0;
            for (size_t i = luca_len; i > 0; --i) {
                seq_t val = luca[i - 1];
                if (val <= rem) {
                    rem -= val;
                    cnt++;
                }
            }

            /* cand is Lucas‑Niven iff cnt > 0 and cand is divisible by cnt */
            if (cnt > 0 && cand % cnt == 0) {
                /* Append to the list */
                if (niven_len == niven_cap) {
                    niven_cap *= 2;
                    niven = (seq_t *)realloc(niven, niven_cap * sizeof(seq_t));
                }
                niven[niven_len++] = cand;
                last_niven = cand;
                break;   /* move on to the next required term */
            }

            cand++;   /* test the next integer */
        }
    }

    return niven[n - 1];
}

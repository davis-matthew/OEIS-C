/*
 * A274016  Choose the lexically first tuple of six nonincreasing positive
 *          integers (a, b, c, d, e, f) such that a*b*c + d*e*f = n. Then a(n)
 *          = a*b*c.
 * https://oeis.org/A274016
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 258, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = minimal lexicographic product a*b*c among all
 * non‑increasing positive integer tuples (a,b,c,d,e,f) with
 * a*b*c + d*e*f = n and a ≥ b ≥ c ≥ d ≥ e ≥ f ≥ 1.
 * The function computes this value for any n ≥ 2.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset starts at 2 */

    bool best_set = false;
    seq_t best[6] = {0};

    /* iterate over all possible (a,b,c) non‑increasing triples */
    idx_t limit_a = n - 1;            /* a*b*c ≤ n-1 */
    for (idx_t a = 1; a <= limit_a; ++a) {
        idx_t max_b = a;
        if ((n - 1) / a < max_b) max_b = (n - 1) / a;
        for (idx_t b = 1; b <= max_b; ++b) {
            idx_t max_c = b;
            idx_t ab = a * b;
            if ((n - 1) / ab < max_c) max_c = (n - 1) / ab;
            for (idx_t c = 1; c <= max_c; ++c) {
                seq_t prod_ab_c = (seq_t)a * (seq_t)b * (seq_t)c;
                if (prod_ab_c > n - 1) continue; /* safety */

                idx_t rem = n - (idx_t)prod_ab_c;  /* remaining for d*e*f, fits in idx_t */

                /* search for non‑increasing (d,e,f) ≤ c such that d*e*f = rem */
                for (idx_t d = 1; d <= c; ++d) {
                    if (rem % d != 0) continue;
                    idx_t rem1 = rem / d;
                    for (idx_t e = 1; e <= d && e <= c; ++e) {
                        if (rem1 % e != 0) continue;
                        idx_t f = rem1 / e;
                        if (f > e || f > c) continue;
                        /* we have a valid tuple */
                        seq_t cand[6] = { a, b, c, d, e, f };

                        if (!best_set) {
                            best_set = true;
                            for (int i = 0; i < 6; ++i) best[i] = cand[i];
                        } else {
                            /* lexicographic comparison */
                            for (int i = 0; i < 6; ++i) {
                                if (cand[i] < best[i]) {
                                    for (int j = 0; j < 6; ++j) best[j] = cand[j];
                                    break;
                                }
                                if (cand[i] > best[i]) break;
                            }
                        }
                    }
                }
            }
        }
    }

    /* If no tuple was found (should not happen for n ≥ 2) */
    if (!best_set) return 0;

    /* Return a*b*c of the best tuple */
    return best[0] * best[1] * best[2];
}

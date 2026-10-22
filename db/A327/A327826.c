/*
 * A327826  Sum of multinomials M(n; lambda), where lambda ranges over all
 *          partitions of n into parts that form a set of size two.
 * https://oeis.org/A327826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p<q} Σ_{A,B>0, A·p + B·q = n} n!/(p!^A q!^B) (sum of
 *            multinomials for partitions of n into only two distinct part
 *            sizes)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* factorial table with increasing size */
static seq_t *fac_tbl = NULL;          /* fac_tbl[0] = 1, fac_tbl[i] = i! */
static idx_t fac_cnt = 0;             /* number of computed factorials */

/* ensure fac_tbl has entries up to n (inclusive) */
static void ensure_factorial(idx_t n)
{
    if (n < fac_cnt) return;
    /* realloc: cast the void* return to seq_t* (required for C++) */
    seq_t *new_tbl = (seq_t *)realloc(fac_tbl, (n + 1) * sizeof(seq_t));
    if (!new_tbl) abort();            /* fatal out‑of‑memory */
    fac_tbl = new_tbl;
    for (idx_t i = fac_cnt; i <= n; ++i) {
        if (i == 0) fac_tbl[i] = (seq_t)1;
        else if (i == 1) fac_tbl[i] = (seq_t)1;
        else fac_tbl[i] = fac_tbl[i - 1] * (seq_t)i;
    }
    fac_cnt = n + 1;
}

/* main sequence function -------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 2) return 0;          /* a(0), a(1), a(2) = 0 */

    ensure_factorial(n);
    const seq_t fn = fac_tbl[n];   /* precomputed n! */
    seq_t sum = 0;

    /* iterate over possible smaller part size p */
    for (idx_t p = 1; p <= n - 1; ++p) {
        /* termP will hold fn / (p!)^A for the current A */
        seq_t termP = fn;
        /* for each multiplicity A of part p */
        for (idx_t A = 1;; ++A) {
            /* stop if remaining sum <= p (needs at least one part of size > p) */
            idx_t rem = n - A * p;
            if (rem <= p) break;
            /* divide by p! once to account for new A */
            termP /= fac_tbl[p];

            /* enumerate all divisors B of rem; each gives a possible q = rem/B */
            for (idx_t d = 1; d * d <= rem; ++d) {
                if (rem % d) continue;

                /* first divisor */
                idx_t B1 = d;
                idx_t q1 = rem / B1;
                if (q1 > p && q1 != p) {
                    seq_t term = termP;
                    for (idx_t i = 0; i < B1; ++i) term /= fac_tbl[q1];
                    sum += term;
                }

                /* second distinct divisor, if any */
                idx_t d2 = rem / d;
                if (d2 != d) {
                    idx_t B2 = d2;
                    idx_t q2 = rem / B2;     /* q2 == d */
                    if (q2 > p && q2 != p) {
                        seq_t term = termP;
                        for (idx_t i = 0; i < B2; ++i) term /= fac_tbl[q2];
                        sum += term;
                    }
                }
            }
        }
    }
    return sum;
}

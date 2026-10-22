/*
 * A335105  Irregular triangle read by rows in which row n lists the partial
 *          sums of shell numbers and respective number of electrons for all
 *          occupied shells of the n-th element of the periodic table of the
 *          elements.
 * https://oeis.org/A335105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, fini
 * Formula:   not stated
 * Range:     u8 n <= 200, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A335105 – irregular triangle of electron‑shell partial sums
 *
 *  The n‑th element of the periodic table is filled according to the
 *  Aufbau principle.  Each occupied subshell is represented by its
 *  principal quantum number (n) and the number of electrons in it.
 *  For a given element we list the pairs (n, e) in increasing n (the
 *  usual spectroscopic order), compute the running cumulative sum of the
 *  sequence n, e, n, e, … and output the values row by row.  The first
 *  term (n = 1) is 1, the second 2, the third 1, etc., as given in the
 *  OEIS b‑file.  This implementation builds the list of all subshells
 *  once, then iteratively constructs the electron configuration for
 *  each element until the requested index is reached.  No hard‑coded
 *  table of terms is used; the algorithm works for any n ≥ 1.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/*  Subshell data structure                                                   */
struct subshell_s {
    int n;          /* principal quantum number   */
    int l;          /* azimuthal quantum number  */
    int cap;        /* capacity (2(2l+1))        */
};

/*  Comparator for Aufbau ordering:  n + l increasing, then n increasing   */
static int subshell_cmp(const void *a, const void *b)
{
    const struct subshell_s *sa = (const struct subshell_s *)a;
    const struct subshell_s *sb = (const struct subshell_s *)b;
    int key_a = sa->n + sa->l;
    int key_b = sb->n + sb->l;
    if (key_a < key_b) return -1;
    if (key_a > key_b) return  1;
    return sa->n - sb->n;
}

/*  Static table of all subshells (s/p/d/f up to n = 1000).  Build once.  */
static bool         subshells_init = false;
static struct subshell_s subshells[4000];
static size_t        nshells = 0;
static const int CAP[4] = {2, 6, 10, 14};

/*  Main function required by the exercise.  */
seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;
    }

    /* Build the global list of subshells if it does not yet exist. */
    if (!subshells_init) {
        const int N_MAX = 1000;           /* covers elements > 102 */
        nshells = 0;

        for (int pq = 1; pq <= N_MAX; ++pq) {
            int lmax;
            if (pq == 1)            lmax = 0;   /* s only            */
            else if (pq == 2)       lmax = 1;   /* s,p only          */
            else                    lmax = 3;   /* s,p,d,f only      */

            for (int l = 0; l <= lmax; ++l) {
                subshells[nshells].n   = pq;
                subshells[nshells].l   = l;
                subshells[nshells].cap = CAP[l];
                ++nshells;
            }
        }
        qsort(subshells, nshells, sizeof(struct subshell_s), subshell_cmp);
        subshells_init = true;
    }

    /*  Walk through elements until the desired index is found.  */
    idx_t idx = 0;     /* current position in the infinite sequence (1‑based) */

    for (idx_t element = 1; ; ++element) {
        idx_t remaining = element;     /* electrons still to place */

        /*  Temporary workspace for the occupied subshells of this element. */
        struct {
            int idx_s;
            int cnt;
        } tmp[64];                         /* 64 > max number of occupied subshells */
        int m = 0;                         /* number of occupied subshells */

        /*  Fill subshells in Aufbau order.  */
        for (size_t s = 0; s < nshells && remaining > 0; ++s) {
            int cap = subshells[s].cap;
            int take = (remaining < (idx_t)cap) ? (int)remaining : cap;
            if (take > 0) {
                tmp[m].idx_s = (int)s;
                tmp[m].cnt   = take;
                ++m;
            }
            remaining -= (idx_t)take;
        }

        /*  Reorder this element’s occupied subshells by increasing n (spectroscopic order).  */
        for (int i = 0; i < m - 1; ++i) {
            for (int j = i + 1; j < m; ++j) {
                const struct subshell_s *si = &subshells[tmp[i].idx_s];
                const struct subshell_s *sj = &subshells[tmp[j].idx_s];
                if (si->n > sj->n || (si->n == sj->n && si->l > sj->l)) {
                    int swap_idx = tmp[i].idx_s;
                    int swap_cnt = tmp[i].cnt;
                    tmp[i].idx_s = tmp[j].idx_s;
                    tmp[i].cnt   = tmp[j].cnt;
                    tmp[j].idx_s = swap_idx;
                    tmp[j].cnt   = swap_cnt;
                }
            }
        }

        /*  Emit the running partial sums for this row.  */
        seq_t row_sum = 0;
        for (int i = 0; i < m; ++i) {
            const struct subshell_s *s = &subshells[tmp[i].idx_s];

            row_sum += (seq_t)s->n;
            ++idx;
            if (idx == n) return row_sum;

            row_sum += (seq_t)(tmp[i].cnt);
            ++idx;
            if (idx == n) return row_sum;
        }
    }
}
